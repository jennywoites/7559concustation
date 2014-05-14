/*
 * Empleado.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: jenniferandrea
 */

#include "Empleado.h"

#include <unistd.h>
#include <stdlib.h>


#include "constantesArchivos.h"

const bool Empleado::USO;
const bool Empleado::DESUSO;

Empleado::Empleado(std::string name, const PipeAutos& generacion, const PipeAutos& atencion, const Semaforo& semaforo) {
	nombre = name;
	cantidadAtendidos = 0;
	arribos = atencion;
	this->generacion = generacion;
	accesoSurtidores = semaforo;
	log.setTipo(Log::ENTRADA_PERSONAJE);
}

Empleado::~Empleado() {}

void Empleado::atenderUnAuto(Auto& autito){
	log.escribirEntrada("Hay auto para ser atendido, patente " + string(autito.getPatente()));
	int surtidor = tomarSurtidor();
	if (surtidor == -1){
		log.escribirEntrada("No pude tomar algun surtidor, entonces el auto  " + autito.getPatente());
		return;
	}
	log.escribirEntrada("Logre tomar el surtidor ",surtidor);

	int litros = autito.llenar();
	log.escribirEntrada("Llene el tanque. Cantidad de litros: ",litros);
	if (devolverSurtidor(surtidor)){
		log.escribirEntrada("He devuelto el surtidor numero ",surtidor);
	}else{
		log.escribirEntrada("No pude devolver el surtidor!! nos quedamos sin el surtidor numero ",surtidor);
	}
	float plata = litros * PRECIO_POR_LITRO;
	caja.depositar(plata);
	log.escribirEntrada("Deposito en caja $", plata);

	log.escribirEntrada("Termine de atender el auto, cuya patente es " + string(autito.getPatente()));
	autito.imprimir(); //Imprimo los datos del auto que fue atendido satisfactoriamente
	try{
		disponibilidad.incrementar(1);
		log.escribirEntrada("Estoy disponible para el jefe");
	}catch(std::string &e){
		log.escribirEntrada("No pude ponerme como disponible" + e); //Igual voy al pipe a leer autos, no termino el proceso por este motivo.
	}

}

bool Empleado::comenzarDia(){
	log.setEscritor("Empleado " + nombre);
	log.escribirEntrada("Se ha iniciado el Proceso.");

	cerrarPipe(generacion, "generacion");

	bool abriCaja = caja.abrir();
	if(!abriCaja){
		log.escribirEntrada("No pude abrir la caja.");
		return false;
	}
	log.escribirEntrada("Abri la caja.");

	try{
		arribos.setearModo(Pipe::LECTURA);
	}catch (std::string &e){
		cout << e << endl;
		log.escribirEntrada("No pude setear modo de comunicacion Lectura");
		cierreDeCaja();
		return false;
	}

	try{
		disponibilidad.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
		disponibilidad.incrementar(1);
		log.escribirEntrada("Me pongo a disposicion del Jefe.");
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("Error en memoria compartida disponibilidad. " + e);
		cierreDeCaja();
		return false;
	}

	return true;
}

void Empleado::cierreDeCaja(){
	bool cerre = caja.cerrar();
	if(!cerre)
		log.escribirEntrada("No pude cerrar la caja.");
	else
		log.escribirEntrada("Cerre la caja.");
}

void Empleado::cerrarPipe(PipeAutos& pipe, const std::string& tipo){
	try{
		pipe.cerrar();
		log.escribirEntrada("Me desadoso del pipe " + tipo);
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("No se pudo desadosar de pipe " + tipo);
	}
}

void Empleado::finalizarDia(){
	cierreDeCaja();
	cerrarPipe(arribos, "atencion");

	try{
		disponibilidad.liberar();
		log.escribirEntrada("Libero la memoria compartida: cantidad de Empleados disponibles.");
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("No fue posible liberar la memoria compartida: cantidad de empleados disponibles");
	}

	for (int i = 0; i < int(surtidores.size()); i++){
		try{
			this->surtidores.at(i).liberar();
			log.escribirEntrada("Libere el surtido numero ", i);
		}catch(std::string &e){
			cout << e << endl;
			log.escribirEntrada("No pude liberar el surtidor numero ", i);
		}
	}

	log.escribirEntrada("Finalice mi proceso.");
}

bool Empleado::crearSurtidores(int cantidadSurtidores){
	for (int i = 0; i < cantidadSurtidores; i++){
		try{
			MemoriaCompartida<bool> surtidor (ARCHIVO_SURTIDORES, SURTIDOR+i);
			this->surtidores.push_back(surtidor);
			//log.escribirEntrada("Asocio surtidor numero: ",i);
		}catch(std::string &e){
			cout << e << endl;
			log.escribirEntrada("No se pudo crear la memoria compartida: Surtidor numero ",i);
			return false;
		}
	}
	return true;
}

pid_t Empleado::atenderAutos(int cantidadSurtidores){
	pid_t id = fork();
	if (id != 0)
		return id;

	bool comienzo = comenzarDia();
	if(!comienzo){
		cerrarPipe(arribos, "atencion");
		log.escribirEntrada("Finalizo Proceso por ERROR.");
		return id;
	}

	Auto autito;

	bool creacion = crearSurtidores(cantidadSurtidores);
	if (! creacion){
		finalizarDia();
		return id;
	}

	while(leerAuto(&autito)){
		atenderUnAuto(autito);
	}

	finalizarDia();

	return id;
}

bool Empleado::leerAuto(Auto* autito){
	log.escribirEntrada("Voy a leer un auto. Si no hay, me duermo.");
	bool status = arribos.leerAuto(autito);
	return status;
}

int Empleado::tomarSurtidor(){
	//Espero a que me dejen solicitar el surtidor. Me habilitan cuando hay un surtidor que pueda tomar
	accesoSurtidores.wait();
	log.escribirEntrada("Estoy buscando un surtidor libre.");

	//busco surtidor que NO este en uso
	try{
		for (unsigned int i = 0; i < surtidores.size(); i++){
			if (surtidores.at(i).modificarValor(USO))
				return i;
			else
				log.escribirEntrada("No logre tomar el surtidor ", int(i));
		}
	}catch(std::string &e){
		cout << e << endl;
		return -1;
	}

	return -1;
}

bool Empleado::devolverSurtidor(int surtidor){
	log.escribirEntrada("Estoy por devolver el surtidor");
	try{
		surtidores.at(surtidor).modificarValor(DESUSO);
		accesoSurtidores.signal();
	}catch(std::string &e){
		cout << e << endl;
		return false;
	}
	return true;
}

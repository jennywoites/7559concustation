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

Empleado::Empleado(std::string name, const PipeAutos& atencion, const Semaforo& semaforo, int tiempoCaja) :
	nombre (name), cantidadAtendidos (0),
	arribos (atencion),
	accesoSurtidores(semaforo),
	tiempoDeposito(tiempoCaja){
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
	cout << nombre + ": utilizo surtidor numero " << surtidor << ", para auto: " + autito.getPatente() << endl;

	int litros = autito.llenar(); //consume un tiempo de llenado
	log.escribirEntrada("Llene el tanque. Cantidad de litros: ",litros);
	if (devolverSurtidor(surtidor)){
		log.escribirEntrada("He devuelto el surtidor numero ",surtidor);
	}else{
		log.escribirEntrada("No pude devolver el surtidor!! nos quedamos sin el surtidor numero ",surtidor);
	}

	float monto = litros * PRECIO_POR_LITRO;
	depositarDinero(monto);

	log.escribirEntrada("Termine de atender el auto, cuya patente es " + string(autito.getPatente()));
	autito.imprimir(); //Imprimo los datos del auto que fue atendido satisfactoriamente
}

void Empleado::depositarDinero(float plata){
	log.escribirEntrada("Deposito en caja $", plata);
	caja.escritorQuiereUsar();
	sleep(tiempoDeposito); // <-- para ver la sincronizacion
	caja.depositar(plata);
	caja.escritorDeja();
	log.escribirEntrada("Ya deposite!");

}

/* incrementa en 1 el numero de empleados disponibles. Si esta activo el flag, se crea la memoria compartida */
bool Empleado::indicarDisponible(bool crear){
	try{
		if (crear)
			disponibilidad.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
		disponibilidad.incrementar(1);
		log.escribirEntrada("Me pongo a disposicion del Jefe.");
	}catch(const std::string &e){
		log.escribirEntrada("Error en memoria compartida disponibilidad: " + e);
		finalizarDia();
		return false;
	}
	return true;
}

bool Empleado::comenzarDia(){
	log.setEscritor("Empleado " + nombre);
	log.escribirEntrada("Se ha iniciado el Proceso.");

	bool abriCaja = caja.abrir();
	if(!abriCaja){
		log.escribirEntrada("No pude abrir la caja.");
		return false;
	}
	log.escribirEntrada("Abri la caja.");

	try{
		arribos.setearModo(Pipe::LECTURA);
	}catch (const std::string &e){
		log.escribirEntrada("No pude setear modo de comunicacion Lectura: " + e);
		finalizarDia();
		return false;
	}

	//indico mi disponibilidad, creando la memoria compartida
	if (! indicarDisponible(true) )
		return false;
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
	}catch(const std::string &e){
		log.escribirEntrada("No se pudo desadosar de pipe " + tipo + ": " + e);
	}
}

void Empleado::finalizarDia(){
	cierreDeCaja();
	cerrarPipe(arribos, "atencion");

	try{
		disponibilidad.liberar();
		log.escribirEntrada("Libero la memoria compartida: cantidad de Empleados disponibles.");
	}catch(const std::string &e){
		log.escribirEntrada("No fue posible liberar la memoria compartida: cantidad de empleados disponibles: " + e);
	}

	for (int i = 0; i < int(surtidores.size()); i++){
		try{
			this->surtidores.at(i).liberar(); //se libera cada surtidor
			log.escribirEntrada("Libere el surtido numero ", i);
		}catch(const std::string &e){
			log.escribirEntrada("No pude liberar: " + e + "; el surtidor numero ", i);
		}
	}

	log.escribirEntrada("Finalice mi proceso.");
}

bool Empleado::crearSurtidores(int cantidadSurtidores){
	for (int i = 0; i < cantidadSurtidores; i++){
		try{
			MemoriaCompartida<bool> surtidor (ARCHIVO_SURTIDORES, SURTIDOR+i); //crea cada surtidor
			this->surtidores.push_back(surtidor); //se almacena en un vector de surtidores
			log.escribirEntrada("Asocio surtidor numero: ",i);
		}catch(const std::string &e){
			log.escribirEntrada("No pudo crear la memoria compartida: " + e + "; del surtidor numero ", i);
			return false;
		}
	}
	return true;
}

pid_t Empleado::atenderAutos(int cantidadSurtidores){
	pid_t id = fork();
	if (id != 0)
		return id;
	//Hijo: Empleado

	bool comienzo = comenzarDia();
	//ha ocurrido un error que no permite continuar con la ejecucion
	if(!comienzo){
		cerrarPipe(arribos, "atencion");
		log.escribirEntrada("Finalizo Proceso por ERROR.");
		return id;
	}

	bool creacion = crearSurtidores(cantidadSurtidores);
	//si la creacion no fue correcta, se cierra el proceso
	if (! creacion){
		finalizarDia();
		return id;
	}

	Auto autito;

	while(leerAuto(autito)){
		atenderUnAuto(autito);
		//si no puedo indicar que estoy disponible, no sigo trabajando
		if (! indicarDisponible(false) ) //no creando la memoria compartida, sino incrementando
			break;
	}

	finalizarDia();

	return id;
}

bool Empleado::leerAuto(Auto& autito){
	log.escribirEntrada("Voy a leer un auto. Si no hay, me duermo.");
	return arribos.leerAuto(autito);
}

int Empleado::tomarSurtidor(){
	//Espero a que me dejen solicitar el surtidor. Me habilitan cuando hay un surtidor que pueda tomar
	accesoSurtidores.wait();
	log.escribirEntrada("Estoy buscando un surtidor libre.");

	//busco surtidor que NO este en uso
	try{
		for (unsigned int i = 0; i < surtidores.size(); i++){
			//que no este en uso y se modifique su valor a uso
			if (surtidores.at(i).modificarValor(USO)) //atomica por semaforo en MemComp
				return i;
			else
				log.escribirEntrada("No logre tomar el surtidor ", int(i));
		}
	}catch(const std::string &e){
		log.escribirEntrada("No logre tomar surtidor alguno: " + e);
		return -1;
	}

	return -1;
}

bool Empleado::devolverSurtidor(int surtidor){
	log.escribirEntrada("Estoy por devolver el surtidor");
	try{
		surtidores.at(surtidor).modificarValor(DESUSO); //atomica por semaforo en MemComp
		accesoSurtidores.signal();
	}catch(const std::string &e){
		log.escribirEntrada("No logre devolver: " + e + "; el surtidor numero ", int(surtidor));
		return false;
	}
	return true;
}

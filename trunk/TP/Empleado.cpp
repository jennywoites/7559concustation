/*
 * Empleado.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: jenniferandrea
 */

#include "Empleado.h"

#include <unistd.h>
#include <stdlib.h>

#include "Log.h"
#include "constantesArchivos.h"

Empleado::Empleado(std::string name, const PipeAutos& pipe) {
	nombre = name;
	cantidadAtendidos = 0;
	arribos = pipe;
}

Empleado::~Empleado() {
}

void Empleado::atenderAutos(int cantidadSurtidores){
	pid_t id = fork();
	if (id != 0)
		return;

	Log::abrir_log();
	Log::setEscritor("Empleado " + nombre);
	Log::enviarMensaje("Se ha iniciado el Proceso.");
	Auto autito;
	caja.abrir();
	Log::enviarMensaje("Abri la caja.");

	disponibilidad.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
	disponibilidad.incrementar(1);
	Log::enviarMensaje("Me pongo a disposicion del Jefe.");

	for (int i = 0; i < cantidadSurtidores; i++){
		MemoriaCompartida<bool> surtidor (ARCHIVO_SURTIDORES, SURTIDOR+i);
		this->surtidores.push_back(surtidor);
		Log::enviarMensaje("Asocio surtidor numero: ",i);
	}

	while(leerAuto(&autito)){

		Log::enviarMensaje("Hay auto para ser atendido, patente " + string(autito.getPatente()));
		int surtidor = tomarSurtidor();
		Log::enviarMensaje("Logre tomar el surtidor ",surtidor);

		int litros = autito.llenar();
		Log::enviarMensaje("Llene el tanque. Cantidad de litros: ",litros);
		devolverSurtidor(surtidor);
		Log::enviarMensaje("He devuelto el surtidor numero ",surtidor);

		float plata = litros * PRECIO_POR_LITRO;
		caja.depositar(plata);
		Log::enviarMensaje("Deposito en caja $", plata);

		Log::enviarMensaje("Termine de atender el auto, cuya patente es " + string(autito.getPatente()));
		autito.imprimir(); //Imprimo los datos del auto que fue atendido satisfactoriamente
		disponibilidad.incrementar(1);
		Log::enviarMensaje("Estoy disponible para el jefe");
	}

	caja.cerrar();
	Log::enviarMensaje("Cerre la caja.");

	disponibilidad.liberar();
	Log::enviarMensaje("Libere el pipe de escritura disponibilidad");

	for (int i = 0; i < int(surtidores.size()); i++){
		this->surtidores.at(i).liberar();
		Log::enviarMensaje("Libere el surtido numero ", i);
	}

	Log::enviarMensaje("Finalice mi proceso correctamente.");
	Log::cerrar_log();
	exit(0);
}

bool Empleado::leerAuto(Auto* autito){
	Log::enviarMensaje("Voy a leer un auto. Si no hay, me duermo.");
	bool status = arribos.leerAuto(autito);
	return status;
}

int Empleado::tomarSurtidor(){
	//llamo a semaforo de surtidores: s.p()
	//busco surtidor que NO este en uso
	for (unsigned int i = 0; i < surtidores.size(); i++){
		if (surtidores.at(i).leer() == DESUSO){ //esto rompe FIXME
			surtidores.at(i).escribir(true);
			return i;
		}
	}
	return -1;
}

void Empleado::devolverSurtidor(int surtidor){
	surtidores.at(surtidor).escribir(false);
	//s.v()
}

/*
 * Empleado.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: jenniferandrea
 */

#include "Empleado.h"
#include "Log.h"
#include "constantesArchivos.h"
#include <unistd.h>
#include <stdlib.h>
#include <sstream>

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
	Log::enviarMensaje("Se ha iniciado el Proceso Empleado "+ nombre + ".");
	Auto autito;
	caja.abrir();
	disponibilidad.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
	disponibilidad.incrementar(1);
	Log::enviarMensaje("Empleado: "+ nombre + " me pongo a disposicion del Jefe.");

	for (int i = 0; i < cantidadSurtidores; i++){
		MemoriaCompartida<bool> surtidor (ARCHIVO_SURTIDORES, SURTIDOR+i);
		this->surtidores.push_back(surtidor);
	}

	while(leerAuto(&autito)){
		Log::enviarMensaje("Empleado: " + nombre + ": Hay auto para ser atendido, patente " + string(autito.getPatente()));
		int surtidor = tomarSurtidor();
		stringstream num;
		num << surtidor;
		Log::enviarMensaje("Empleado: " + nombre + ": Logre tomar el surtidor " + num.str());
		int litros = autito.llenar();
		devolverSurtidor(surtidor);
		Log::enviarMensaje("Empleado: " + nombre + ": He devuelto el surtidor " + num.str());
		float plata = litros * PRECIO_POR_LITRO;
		caja.depositar(plata);
		num << plata;
		Log::enviarMensaje("Empleado: " + nombre + ": Deposito en caja " + num.str());
		disponibilidad.incrementar(1);
		Log::enviarMensaje("Empleado: " + nombre + ": Me pongo en disponibilidad ");
	}

	caja.cerrar();
	disponibilidad.liberar();
	for (unsigned int i = 0; i < surtidores.size(); i++){
		this->surtidores.at(i).liberar();
	}

	Log::cerrar_log();
	exit(0);
}

bool Empleado::leerAuto(Auto* autito){
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

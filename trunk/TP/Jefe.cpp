/*
 * Jefe.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "Jefe.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "constantesArchivos.h"
#include "Log.h"

Jefe::Jefe(std::string n, const PipeAutos& gen, const PipeAutos& aten) {
	nombre = n;
	cantidadAtendida = 0;
	cantidadDespachada = 0;

	arribos = gen;
	envios = aten;
}

Jefe::~Jefe() {
}

bool Jefe::leerAuto(Auto* autito){
	bool status = arribos.leerAuto(autito);
	return status;
}

void Jefe::atenderAutos(){
	pid_t id = fork();
	if (id != 0)
		return;

	Log::abrir_log();
	Auto autito;

	Log::enviarMensaje("Se ha iniciado el Proceso Jefe "+ nombre + ".");

	cantEmpleadosDisponibles.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);

	while (leerAuto(&autito)){
		Log::enviarMensaje("Jefe " + nombre + ": Hay auto para ser atendido, patente " + string(autito.getPatente()));
		if (!hayEmpleados()){
			cantidadDespachada++;
			mensajeDespachante(autito.getPatente());
		}else{
			cantidadAtendida++;
			tomarEmpleado();
			enviarAutoAEmpleado(autito);
		}
	}

	arribos.cerrar();
	Log::enviarMensaje("Jefe cierra el pipe de lectura arribos");
	envios.cerrar();
	Log::enviarMensaje("Jefe cierra el pipe de escritura envios");
	Log::enviarMensaje("Fin del proceso Jefe");

	cantEmpleadosDisponibles.liberar();

	Log::cerrar_log();
	exit(0);
}

bool Jefe::hayEmpleados(){
	int cant_empleados = cantEmpleadosDisponibles.leer();
	stringstream ss;
	ss << cant_empleados;
	Log::enviarMensaje("Jefe: cantidad de empleados disponibles es " + ss.str());
	return (cant_empleados > 0);
}

void Jefe::tomarEmpleado(){
	Log::enviarMensaje("Jefe toma un empleado disponible.");
	cantEmpleadosDisponibles.incrementar(-1);
}

void Jefe::mensajeDespachante(std::string patente){
	Log::enviarMensaje("Jefe " + nombre + ": " + " Se ha despachado al auto de patente " + patente + " por no haber empleados libres");
}

bool Jefe::enviarAutoAEmpleado(const Auto& autito){
	Log::enviarMensaje("Jefe envia el auto por el pipe hacia empleados");
	return envios.escribirAuto(autito);
}

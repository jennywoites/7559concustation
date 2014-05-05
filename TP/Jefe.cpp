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

#include "constantesArchivos.h"
#include "Log.h"

Jefe::Jefe(std::string n, const PipeAutos& gen, const PipeAutos& aten) {
	nombre = n;
	cantidadAtendida = 0;
	cantidadDespachada = 0;

	arribos = gen;
	envios = aten;
}

Jefe::~Jefe(){}

void Jefe::printDebug(std::string msj){
	Log::enviarMensaje("Jefe " + nombre + ": " + msj);
}

void Jefe::printDebug(std::string msj, int numero){
	Log::enviarMensaje("Jefe " + nombre + ": " + msj, numero);
}


bool Jefe::leerAuto(Auto* autito){
	bool status = arribos.leerAuto(autito);
	printDebug("Lei un auto.");
	return status;
}

void Jefe::atenderAutos(){
	pid_t id = fork();
	if (id != 0)
		return;

	Log::abrir_log();
	Auto autito;

	printDebug("Se ha iniciado el Proceso.");

	cantEmpleadosDisponibles.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
	printDebug("Se creo la memoria compartida: cantidad de empleados disponibles.");

	while (leerAuto(&autito)){
		printDebug("Hay auto para ser atendido, patente " + string(autito.getPatente()));
		if (!hayEmpleados()){
			cantidadDespachada++;
			printDebug("La cantidad de autos despachados es de: ", cantidadDespachada);
			mensajeDespachante(autito.getPatente());
		}else{
			cantidadAtendida++;
			tomarEmpleado();
			enviarAutoAEmpleado(autito);
			printDebug("La cantidad de autos que atendi es de: ", cantidadAtendida);
		}
	}

	arribos.cerrar();
	printDebug("Cierro el pipe de lectura arribos");
	envios.cerrar();
	printDebug("Cierro el pipe de escritura envios");

	cantEmpleadosDisponibles.liberar();
	printDebug("Libero la memoria compartida: cantidad de Empleados disponibles.");

	printDebug("Fin del proceso.");
	Log::cerrar_log();
	exit(0);
}

bool Jefe::hayEmpleados(){
	int cant_empleados = cantEmpleadosDisponibles.leer();
	printDebug("La cantidad de empleados disponibles es ", cant_empleados);
	return (cant_empleados > 0);
}

void Jefe::tomarEmpleado(){
	printDebug("Tomo un empleado disponible.");
	cantEmpleadosDisponibles.incrementar(-1);
}

void Jefe::mensajeDespachante(std::string patente){
	printDebug("Se ha despachado al auto de patente " + patente + " por no haber empleados libres");
}

bool Jefe::enviarAutoAEmpleado(const Auto& autito){
	printDebug("Envio el auto por el pipe hacia empleados");
	return envios.escribirAuto(autito);
}

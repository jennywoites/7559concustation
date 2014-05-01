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
#include "constantesArchivos.h"

Jefe::Jefe(std::string n, const PipeAutos& canal) {
	nombre = n;
	cantidadAtendida = 0;
	cantidadDespachada = 0;

	arribos = canal;

	cantEmpleadosDisponibles = MemoriaCompartida<int>(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
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

	Auto autito;

	while (leerAuto(&autito)){
		if (!hayEmpleados()){
			cantidadDespachada++;
			mensajeDespachante(autito.getPatente());
			continue;
		}
		tomarEmpleado();
		enviarAutoAEmpleado(&autito);
	}
	exit(0);
}

bool Jefe::hayEmpleados(){
	return (cantEmpleadosDisponibles.leer() > 0);
}

void Jefe::tomarEmpleado(){
	cantEmpleadosDisponibles.incrementar(-1);
}

void Jefe::mensajeDespachante(std::string patente){
	std::string mensaje = "Jefe " + nombre + ": ";
	mensaje += " Se ha despachado al auto de patente ";
	mensaje += patente;
	mensaje	+= " por no haber empelados libres\n";
	cout << mensaje;
}

void Jefe::enviarAutoAEmpleado(const Auto* autito){

}

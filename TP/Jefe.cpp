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

Jefe::Jefe(std::string n, const PipeAutos& gen, const PipeAutos& aten) {
	nombre = n;
	cantidadAtendida = 0;
	cantidadDespachada = 0;

	arribos = gen;
	envios = aten;
}

Jefe::~Jefe(){}

bool Jefe::leerAuto(Auto* autito){
	log.escribirEntrada("Voy a leer un auto. Si no hay, me duermo.");
	bool status = arribos.leerAuto(autito);
	return status;
}

void Jefe::atenderAutos(){
	pid_t id = fork();
	if (id != 0)
		return;

	log.setEscritor("Jefe " + nombre);
	Auto autito;

	arribos.setearModo(Pipe::LECTURA);
	envios.setearModo(Pipe::ESCRITURA);

	log.escribirEntrada("Se ha iniciado el Proceso.");

	cantEmpleadosDisponibles.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
	log.escribirEntrada("Se creo la memoria compartida: cantidad de empleados disponibles.");

	while (leerAuto(&autito)){
		log.escribirEntrada("Hay auto para ser atendido, patente " + string(autito.getPatente()));
		if (!hayEmpleados()){
			cantidadDespachada++;
			log.escribirEntrada("La cantidad de autos despachados es de: ", cantidadDespachada);
			mensajeDespachante(autito.getPatente());
		}else{
			cantidadAtendida++;
			tomarEmpleado();
			enviarAutoAEmpleado(autito);
			log.escribirEntrada("La cantidad de autos que atendi es de: ", cantidadAtendida);
		}
	}

	arribos.cerrar();
	log.escribirEntrada("Cierro el pipe de lectura arribos");
	envios.cerrar();
	log.escribirEntrada("Cierro el pipe de escritura envios");

	cantEmpleadosDisponibles.liberar();
	log.escribirEntrada("Libero la memoria compartida: cantidad de Empleados disponibles.");

	log.escribirEntrada("Fin del proceso.");
	exit(0);
}

bool Jefe::hayEmpleados(){
	int cant_empleados = cantEmpleadosDisponibles.leer();
	log.escribirEntrada("La cantidad de empleados disponibles es ", cant_empleados);
	return (cant_empleados > 0);
}

void Jefe::tomarEmpleado(){
	log.escribirEntrada("Tomo un empleado disponible.");
	cantEmpleadosDisponibles.incrementar(-1);
}

void Jefe::mensajeDespachante(std::string patente){
	log.escribirEntrada("Se ha despachado al auto de patente " + patente + " por no haber empleados libres");
}

bool Jefe::enviarAutoAEmpleado(const Auto& autito){
	log.escribirEntrada("Envio el auto por el pipe hacia empleados");
	return envios.escribirAuto(autito);
}

/*
 * Empleado.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: jenniferandrea
 */

#include "Empleado.h"
#include "constantesArchivos.h"
#include <unistd.h>
#include <stdlib.h>

Empleado::Empleado(std::string name, int cantidadSurtidores, const PipeAutos& pipe) {
	nombre = name;
	cantidadAtendidos = 0;

	for (int i = 0; i < cantidadSurtidores; i++){
		MemoriaCompartida<bool> surtidor (ARCHIVO_SURTIDORES, SURTIDOR+i);
		this->surtidores.push_back(surtidor);
	}
	arribos = pipe;
}

Empleado::~Empleado() {
}

void Empleado::atenderAutos(){
	pid_t id = fork();
	if (id != 0)
		return;

	Auto autito;
	caja.abrir();
	disponibilidad.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
	disponibilidad.incrementar(1);
	while(leerAuto(&autito)){
		cout << autito.getPatente() << endl;
		disponibilidad.incrementar(1);
		caja.depositar(100);
		continue;

		int surtidor = tomarSurtidor();
		int litros = autito.llenar();
		devolverSurtidor(surtidor);
		float plata = litros * PRECIO_POR_LITRO;
		caja.depositar(plata);
		disponibilidad.incrementar(1);
	}
	caja.cerrar();
	disponibilidad.liberar();
	for (unsigned int i = 0; i < surtidores.size(); i++){
		this->surtidores.at(i).liberar();
	}

	exit(0);
}

bool Empleado::leerAuto(Auto* autito){
	bool status = arribos.leerAuto(autito);
	return status;
}

int Empleado::tomarSurtidor(){
	//llamo a semaforo de surtidores: s.p()
	for (unsigned int i = 0; i < surtidores.size(); i++){
		if (surtidores.at(i).leer()){ //esto rompe FIXME
			surtidores.at(i).escribir(false);
			return i;
		}
	}
	return -1;
}

void Empleado::devolverSurtidor(int surtidor){
	surtidores.at(surtidor).escribir(true);
	//s.v()
}

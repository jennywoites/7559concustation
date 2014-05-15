/*
 * PipeAutos.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "PipeAutos.h"
#include <iostream>
#include <string.h>
#include "../Log.h"

PipeAutos::PipeAutos() {
}

bool PipeAutos::crear(const std::string& nombre) {
	try{
		Pipe::crear();
		//semaforo binario que controla el extremo de lectura del pipe
		controlLectura.crear(nombre, PIPE, UN_SEMAFORO);
	}catch(std::string &e){
		cerr << "No es posible crear el Pipe Autos: " + e << endl;
		return false;
	}
	return true;
}

PipeAutos::~PipeAutos() {}

bool PipeAutos :: leerAuto ( Auto& autito ) {
	char serie[Auto::LONG_SERIE];
	//se hace wait para ser el unico que realiza la lectura del pipe
	controlLectura.wait();

	ssize_t leido;
	try{
		leido = Pipe::leer(static_cast<void*>(serie), Auto::LONG_SERIE);
	}catch(const std::string &e){
		cerr << "No pudo leerse auto desde el Pipe: " + e << endl;
		controlLectura.signal();
		return false;
	}

	controlLectura.signal();
	if (leido != Auto::LONG_SERIE)
		return false;

	autito.deserializar(serie);
	return true;
}

bool PipeAutos :: escribirAuto ( const Auto& autito ) {
	std::string serie = autito.serializar();
	ssize_t escrito;
	try{
		escrito = Pipe::escribir(static_cast<const void*>(serie.c_str()), Auto::LONG_SERIE);
	}catch(const std::string &e){
		cerr << "No pudo escribirse auto hacia el Pipe: " + e << endl;
		return false;
	}
	return (escrito == Auto::LONG_SERIE);
}

void PipeAutos::liberar(){
	controlLectura.eliminar();
}

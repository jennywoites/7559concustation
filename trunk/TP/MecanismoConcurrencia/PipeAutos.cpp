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

void PipeAutos::crear(const std::string& nombre) {
	Pipe::crear();
	controlLectura.crear(nombre, PIPE, UN_SEMAFORO);
}

PipeAutos::~PipeAutos() {}

bool PipeAutos :: leerAuto ( Auto* autito ) {
	char serie[Auto::LONG_SERIE];
	//se hace wait para ser el unico que realiza la lectura del pipe
	controlLectura.wait();

	ssize_t leido;
	try{
		leido = Pipe::leer(static_cast<void*>(serie), Auto::LONG_SERIE);
	}catch(const std::string &e){
		cerr << e << endl;
		controlLectura.signal();
		return 0;
	}

	controlLectura.signal();
	if (leido != Auto::LONG_SERIE)
		return false;

	autito->deserializar(serie);
	return true;
}

bool PipeAutos :: escribirAuto ( const Auto& autito ) {
	std::string serie = autito.serializar();
	ssize_t escrito;
	try{
		escrito = Pipe::escribir(static_cast<const void*>(serie.c_str()), Auto::LONG_SERIE);
	}catch(const std::string &e){
		cerr << e << endl;
		return 0;
	}
	return (escrito == Auto::LONG_SERIE);
}

void PipeAutos::liberar(){
	controlLectura.eliminar();
}

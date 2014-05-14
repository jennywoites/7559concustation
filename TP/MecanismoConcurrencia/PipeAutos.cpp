/*
 * PipeAutos.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "PipeAutos.h"
#include <iostream>
#include "../Log.h"

PipeAutos::PipeAutos() {
}

void PipeAutos::crear(const std::string& nombre) {
	Pipe::crear();
	controlLectura.crear(nombre, PIPE, UN_SEMAFORO);
}

PipeAutos::~PipeAutos() {}

bool PipeAutos :: leerAuto ( Auto* autito ) {
	std::string serie;
	//Log::enviarMensaje("Debug: valor de Semaph ", controlLectura.getVal());
	//Log::enviarMensaje("hago wait");
	controlLectura.wait();
	//Log::enviarMensaje("ingrese pipeLectura");
	//Log::enviarMensaje("Debug: valor de Semaph ", controlLectura.getVal());
	ssize_t leido = Pipe::leer(static_cast<void*>(&serie), Auto::LONG_SERIE);
	//Log::enviarMensaje("hago signal");
	controlLectura.signal();
	//Log::enviarMensaje("Debug: valor de Semaph liberado ", controlLectura.getVal());
	if (leido != Auto::LONG_SERIE)
		return false;

	autito->deserializar(serie);
	return true;
}

bool PipeAutos :: escribirAuto ( const Auto& autito ) {
	std::string serie = autito.serializar();
	ssize_t escrito = Pipe::escribir(static_cast<const void*>(&serie), Auto::LONG_SERIE);
	return (escrito == Auto::LONG_SERIE);
}

void PipeAutos::liberar(){
	controlLectura.eliminar();
}

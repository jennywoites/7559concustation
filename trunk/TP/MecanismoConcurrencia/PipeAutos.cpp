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
	//struct auto_serial serie;
	//Log::enviarMensaje("Debug: valor de Semaph ", controlLectura.getVal());
	//Log::enviarMensaje("hago wait");
	controlLectura.wait();
	//Log::enviarMensaje("ingrese pipeLectura");
	//Log::enviarMensaje("Debug: valor de Semaph ", controlLectura.getVal());
	ssize_t leido;
	try{
		leido = Pipe::leer(static_cast<void*>(serie), Auto::LONG_SERIE);
		//leido = Pipe::leer(static_cast<void*>(&serie), sizeof(struct auto_serial));
	}catch(std::string &e){
		controlLectura.signal();
		return 0;
	}

	//Log::enviarMensaje("hago signal");
	controlLectura.signal();
	//Log::enviarMensaje("Debug: valor de Semaph liberado ", controlLectura.getVal());
	if (leido != Auto::LONG_SERIE)
	//if (leido != sizeof(struct auto_serial))
		return false;

	autito->deserializar(serie);
	return true;
}

bool PipeAutos :: escribirAuto ( const Auto& autito ) {
	std::string serie = autito.serializar();
	//struct auto_serial serie = autito.serializar();
	ssize_t escrito;
	try{
		escrito = Pipe::escribir(static_cast<const void*>(serie.c_str()), Auto::LONG_SERIE);
		//escrito = Pipe::escribir(static_cast<const void*>(&serie), sizeof(struct auto_serial));
	}catch(std::string &e){
		cout << e << endl;
		return 0;
	}
	//return (escrito == sizeof(struct auto_serial));
	return (escrito == Auto::LONG_SERIE);
}

void PipeAutos::liberar(){
	controlLectura.eliminar();
}

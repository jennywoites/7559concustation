/*
 * PipeAutos.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "PipeAutos.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "../Log.h"

using namespace std;

PipeAutos::PipeAutos() {
	// TODO Auto-generated constructor stub

}

void PipeAutos::crear(const std::string& nombre) {
	Pipe::crear();
	controlLectura.crear(nombre, PIPE, 1);
	//FIXME
}

PipeAutos::~PipeAutos() {
	// TODO Auto-generated destructor stub
}

bool PipeAutos :: leerAuto ( Auto* autito ) {
	struct auto_serial serie;
	Log::enviarMensaje("Debug: valor de Semaph ", controlLectura.getVal());
	Log::enviarMensaje("hago wait");
	controlLectura.wait();
	Log::enviarMensaje("ingrese pipeLectura");
	Log::enviarMensaje("Debug: valor de Semaph ", controlLectura.getVal());
	ssize_t leido = Pipe::leer(static_cast<void*>(&serie), sizeof(struct auto_serial));
	Log::enviarMensaje("hago signal");
	controlLectura.signal();
	Log::enviarMensaje("Debug: valor de Semaph liberado ", controlLectura.getVal());
	if (leido!=sizeof(struct auto_serial))
		return false;

	autito->deserializar(serie);
	return true;
}

bool PipeAutos :: escribirAuto ( const Auto& autito ) {
	struct auto_serial serie = autito.serializar();
	ssize_t escrito = Pipe::escribir(static_cast<const void*>(&serie), sizeof(struct auto_serial));
	return (escrito==sizeof(struct auto_serial));
}

void PipeAutos::cerrar(){
	Pipe::cerrar();
}

void PipeAutos::liberar(){
	controlLectura.eliminar();
}

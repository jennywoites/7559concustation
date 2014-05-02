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
using namespace std;

PipeAutos::PipeAutos() {
	// TODO Auto-generated constructor stub

}

PipeAutos::~PipeAutos() {
	// TODO Auto-generated destructor stub
}

bool PipeAutos :: leerAuto ( Auto* autito ) {
	struct auto_serial serie;
	ssize_t leido = Pipe::leer(static_cast<void*>(&serie), sizeof(struct auto_serial));
	if (leido!=sizeof(struct auto_serial))
		return false;

	autito->deserializar(serie);
	return true;
}

bool PipeAutos :: escribirAuto ( const Auto autito ) {
	struct auto_serial serie = autito.serializar();
	ssize_t escrito = Pipe::escribir(static_cast<const void*>(&serie), sizeof(struct auto_serial));
	return (escrito==sizeof(struct auto_serial));
}

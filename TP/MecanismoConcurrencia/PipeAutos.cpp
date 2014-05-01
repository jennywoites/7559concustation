/*
 * PipeAutos.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "PipeAutos.h"
#include "../structAuto.h"
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
	char serial[7];
	ssize_t leido = Pipe::leer(static_cast<void*>(serial), sizeof(serial));
	//FIXME: des-serializar
	*autito = Auto(serial, 10);
	return (leido==sizeof(serial));
}

bool PipeAutos :: escribirAuto ( const Auto autito ) {
	//FIXME: serializar
	char serial[7];
	strcpy(serial, "FIXME!");
	ssize_t escrito = Pipe::escribir(static_cast<const void*>(serial), sizeof(serial));
	return (escrito==sizeof(serial));
}

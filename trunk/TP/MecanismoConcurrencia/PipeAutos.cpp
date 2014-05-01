/*
 * PipeAutos.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "PipeAutos.h"

using namespace std;

PipeAutos::PipeAutos() {
	// TODO Auto-generated constructor stub

}

PipeAutos::~PipeAutos() {
	// TODO Auto-generated destructor stub
}

bool PipeAutos :: leerAuto ( Auto* autito ) {
	ssize_t leido = Pipe::leer(static_cast<void*>(autito), sizeof(*autito));
	return (leido==sizeof(*autito));
}

bool PipeAutos :: escribirAuto ( const Auto autito ) {
	return Pipe::escribir(static_cast<const void*>(&autito), sizeof(autito));
}

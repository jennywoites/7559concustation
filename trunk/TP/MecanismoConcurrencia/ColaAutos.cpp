/*
 * ColaAutos.cpp
 *
 *  Created on: 21/06/2014
 *      Author: jennifer
 */

#include "ColaAutos.h"
#include <iostream>
#include <string.h>
#include "../Log.h"

ColaAutos::ColaAutos() {
}

bool ColaAutos::crear(const std::string& nombre) {
	try{
		envio.crear(nombre, COLA);
	}catch(std::string &e){
		cerr << "No es posible crear el Cola Autos: " + e << endl;
		return false;
	}
	return true;
}

ColaAutos::~ColaAutos() {}

bool ColaAutos :: leerAuto ( Auto& autito ) {
	Auto::auto_t serie;

	ssize_t leido;
	try{
		leido = envio.leer(-1*Auto::PRIOR_MAX, &serie);
	}catch(const std::string &e){
		cerr << "No pudo leerse auto desde Cola: " + e << endl;
		return false;
	}

	if (leido + sizeof(long) != sizeof(Auto::auto_t))
		return false;

	autito.reconstruir(serie);
	return true;
}

bool ColaAutos :: escribirAuto ( const Auto& autito ) {
	const Auto::auto_t serie = autito.obtenerEstructura();
	ssize_t escrito;
	try{
		escrito = envio.escribir(serie);
	}catch(const std::string &e){
		cerr << "No pudo escribirse auto hacia Cola: " + e << endl;
		return false;
	}
	return (escrito == 0);
}

void ColaAutos::liberar(){
	envio.destruir();
}

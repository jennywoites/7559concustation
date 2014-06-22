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
		//FIXME
		leido = envio.leer(-10, &serie);
	}catch(const std::string &e){
		cerr << "No pudo leerse auto desde el Cola: " + e << endl;
		return false;
	}

	if (leido != sizeof(Auto::auto_t))
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
		cerr << "No pudo escribirse auto hacia el Cola: " + e << endl;
		return false;
	}
	return (escrito == Auto::LONG_SERIE);
}

void ColaAutos::liberar(){
	envio.destruir();
}

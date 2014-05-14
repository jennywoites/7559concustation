/*
 * Caja.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include "Caja.h"
#include "constantesArchivos.h"

using namespace std;

Caja::Caja() {}

Caja::~Caja() {}

void Caja::depositar(float plata_a_agregar){
	if (plata_a_agregar < 0 ){
		//deberia lanzar una excepcion
		return;
	}
	plata.incrementar(plata_a_agregar);
}

float Caja::verMonto() const{
	return plata.leer();
}

bool Caja::cerrar(){
	try{
		plata.liberar();
	}catch(std::string &e){
		cout << e << endl;
		return false;
	}
	return true;
}

bool Caja::abrir(){
	try{
		plata.crear(ARCHIVO_CAJA, CAJA);
	}catch(std::string &e){
		cout << e << endl;
		return false;
	}
	return true;
}


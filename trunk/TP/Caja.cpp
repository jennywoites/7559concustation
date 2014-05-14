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
		return;
	}
	try{
		plata.incrementar(plata_a_agregar);
	}catch(std::string &e){
		cout << "No es posible depositar en caja. " << e << endl;
	}
}

float Caja::verMonto() const{
	float dinero = 0;
	try{
		dinero = plata.leer();
	}catch(std::string &e){
		cout << "No es posible ver monto de la caja. " << e << endl;
	}

	return dinero;
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


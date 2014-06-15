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
	}catch(const std::string &e){
		cerr << "No fue posible depositar en caja. " << e << endl;
	}
}

float Caja::verMonto() const{
	float dinero = 0;
	try{
		dinero = plata.leer();
	}catch(const std::string &e){
		cerr << "No es posible ver monto de la caja. " << e << endl;
		return -1;
	}

	return dinero;
}

bool Caja::cerrar(){
	try{
		plata.liberar();
	}catch(const std::string &e){
		cerr << "No fue posible liberar la caja. " << e << endl;
		return false;
	}
	return true;
}

bool Caja::abrir(){
	try{
		plata.crear(ARCHIVO_CAJA, CAJA);
		vip.crear(ARCHIVO_CAJA, CAJA+1, 1);
		escritores.crear(ARCHIVO_CAJA, CAJA+2, 1);
	}catch(std::string &e){
		cerr << "No es posible abrir la caja. " << e << endl;
		return false;
	}
	return true;
}


void Caja::lectorQuiereUsar(){
	vip.wait();
}

void Caja::lectorDeja(){
	vip.signal();
}

void Caja::escritorQuiereUsar(){
	escritores.wait();
	vip.wait();
}

void Caja::escritorDeja(){
	vip.signal();
	escritores.signal();
}

void Caja::eliminar(){
	escritores.eliminar();
	vip.eliminar();
}

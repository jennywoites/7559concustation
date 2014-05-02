/*
 * Caja.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include "Caja.h"
#include "constantesArchivos.h"

Caja::Caja() {
}

Caja::~Caja() {
}

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

void Caja::cerrar(){
	plata.liberar();
}

void Caja::abrir(){
	plata.crear(ARCHIVO_CAJA, CAJA);
}

void Caja::abrir(float plataInicial){
	plata.crear(ARCHIVO_CAJA, CAJA);
	plata.escribir(plataInicial);
}

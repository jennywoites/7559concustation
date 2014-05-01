/*
 * Caja.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include "Caja.h"
#include "constantesArchivos.h"

Caja::Caja() {
	plata = MemoriaCompartida<float>(ARCHIVO_CAJA, CAJA);
}

Caja::Caja(float plataInicial){
	plata = MemoriaCompartida<float>(ARCHIVO_CAJA, CAJA);
	plata.escribir(plataInicial);
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
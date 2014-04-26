/*
 * Caja.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include "Caja.h"

Caja::Caja() {
	plata = 0;
}

Caja::Caja(int plataInicial){
	plata = plataInicial;
}

Caja::~Caja() {

}

void Caja::depositar(float plata_a_agregar){
	if (plata_a_agregar < 0 ){
		//deberia lanzar una excepcion
		return;
	}
	plata += plata_a_agregar;
}

float Caja::verMonto() const{
	return plata;
}

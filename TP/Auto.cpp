/*
 * Auto.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include "Auto.h"
#include "ManejoTiempos.h"

Auto::Auto() {
	patente = generarPatenteRandom();
	capacidadTanque = generarTanqueRandom();
	lleno = false;
}

Auto::~Auto() {

}

std::string Auto::getPatente() const{
	return patente;
}

int Auto::llenar(){
	if (lleno)
		return 0;

	int tiempoPorLitro = numeroAlAzar(tiempoPorLitroMin, tiempoPorLitroMax);
	float tiempo = (float)(tiempoPorLitro * capacidadTanque);
	//Esperamos a que se termine el tiempo determinado:
	usleep(tiempo);

	lleno = true;
	return capacidadTanque;
}

int Auto::generarTanqueRandom() const{
	return numeroAlAzar(capacidadMin,capacidadMax);
}

std::string Auto::generarPatenteRandom() const{
	std::string pat = "";
	for (unsigned int i = 0; i < cantidadLetras; i++){
		pat += 'A' + numeroAlAzar(0,25);
	}
	for (unsigned int i = 0; i < cantidadNumeros; i++){
		pat += '0' + numeroAlAzar(0,9);
	}
	return pat;
}

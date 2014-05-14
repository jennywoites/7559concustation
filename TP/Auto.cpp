/*
 * Auto.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include "Auto.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "ManejoTiempos.h"

using namespace std;

Auto::Auto() {
	patente = generarPatenteRandom();
	capacidadTanque = generarTanqueRandom();
	lleno = false;
}

Auto::Auto(std::string patente, int capacidad){
	this->capacidadTanque = capacidad;
	this->patente = patente;
	this->lleno = false;
}

void Auto::imprimir(){
	cout << "Se atendio el auto, cuya patente es: " << patente << ". Se le cargaron " << capacidadTanque << " litros." << endl;
	//TODO: deberia ir al Log
}

Auto::~Auto() {}

std::string Auto::getPatente() const{
	return patente;
}

int Auto::llenar(){
	if (lleno)
		return 0;

	int tiempoPorLitro = numeroAlAzar(TIEMPO_POR_LITRO_MIN, TIEMPO_POR_LITRO_MAX);
	int tiempo = tiempoPorLitro * capacidadTanque;
	//Esperamos a que se termine el tiempo determinado:
	usleep(tiempo);

	lleno = true;
	return capacidadTanque;
}

int Auto::generarTanqueRandom() const{
	return numeroAlAzar(CAPACIDAD_MIN,CAPACIDAD_MAX);
}

std::string Auto::generarPatenteRandom() const{
	std::string pat = "";
	for (unsigned int i = 0; i < CANTIDAD_LETRAS; i++){
		pat += 'A' + numeroAlAzar(0,25);
	}
	for (unsigned int i = 0; i < CANTIDAD_NUMEROS; i++){
		pat += '0' + numeroAlAzar(0,9);
	}
	return pat;
}

struct auto_serial Auto::serializar() const{
	struct auto_serial s;
	memset(static_cast<void*>(&s), 0, sizeof(struct auto_serial));
	s.capacidad = capacidadTanque;
	s.lleno = lleno;
	strcpy(s.patente, patente.c_str());
	return s;
}

void Auto::deserializar(struct auto_serial serie){
	lleno = serie.lleno;
	capacidadTanque = serie.capacidad;
	patente = serie.patente;
}

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
#include <cstring>
#include <sstream>
#include <iostream>

#include "ManejoTiempos.h"

using namespace std;

#define SEPARADOR ";"
#define TRUE 1
#define FALSE 0


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

std::string Auto::serializar() const{
	std::string serial = "";
	serial += patente;
	serial += SEPARADOR;
	stringstream cap,llen;
	cap << capacidadTanque;
	serial += cap.str();
	serial += SEPARADOR;
	if(lleno)
		llen << TRUE;
	else
		llen << FALSE;
	serial += llen.str();
	return serial;
}

void Auto::deserializar(const std::string& serial){
	char* serie = (char*) malloc(LONG_SERIE);
	if(!serie){
		patente = "ERR-DS";
		capacidadTanque = 0;
		lleno = true;
		return;
	}
	strcpy(serie,serial.c_str());
	patente = strtok(serie,SEPARADOR);
	capacidadTanque = atoi(strtok(NULL,SEPARADOR));
	lleno = atoi(strtok(NULL,SEPARADOR));
	free(serie);
}

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
#include <sstream>
#include <iostream>

#include "ManejoTiempos.h"

using namespace std;

#define SEPARADOR ";"
#define TRUE 1
#define FALSE 0


Auto::Auto():
	patente ( generarPatenteRandom() ),
	capacidadTanque ( generarTanqueRandom() ),
	lleno (false) {}

Auto::Auto(std::string patente, int capacidad):
	patente(patente),
	capacidadTanque(capacidad),
	lleno(false) {}

void Auto::imprimir() const{
	cout << "\033[1;34m" << "Se atendio el auto, cuya patente es: " + patente +  ". Se le cargaron " << capacidadTanque << " litros." << "\033[0m" << endl;
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
	std::string chapaPatente = "";
	for (unsigned int i = 0; i < CANTIDAD_LETRAS; i++){
		chapaPatente += 'A' + numeroAlAzar(0,25);
	}
	for (unsigned int i = 0; i < CANTIDAD_NUMEROS; i++){
		chapaPatente += '0' + numeroAlAzar(0,9);
	}
	return chapaPatente;
}

std::string Auto::serializar() const{
	//une, con separadores, los campos de
	std::string serial = "";
	//patente
	serial += patente + SEPARADOR;
	stringstream cap, llen;
	//capacidad del tanque
	cap << capacidadTanque;
	serial += cap.str() + SEPARADOR;
	//lleno
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
	//se toman los campos de acuerdo al separador
	patente = strtok(serie,SEPARADOR);
	capacidadTanque = atoi(strtok(NULL,SEPARADOR));
	lleno = atoi(strtok(NULL,SEPARADOR));
	free(serie);
}

/*struct auto_t Auto::obtenerEstructura(long tipo){
	struct auto_t estructura;
	estructura.capacidad = capacidadTanque;
	estructura.estaLleno = lleno;
	estructura.patente_auto = patente;
	estructura.mtype = tipo;
	return estructura;
}*/

void Auto::reconstruir(struct auto_t autito){
	capacidadTanque = autito.capacidad;
	lleno = autito.estaLleno;
	patente = autito.patente_auto;
}

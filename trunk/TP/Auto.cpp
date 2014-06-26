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
	lleno (false),
	prioridad(generarPrioridadRandom()){}

Auto::Auto(std::string patente, int capacidad, long priority = PRIOR_MAX):
	patente(patente),
	capacidadTanque(capacidad),
	lleno(false),
	prioridad(priority){}

void Auto::imprimir() const{
	cout << "\033[1;34m" << "Se atendio el auto, cuya patente es: " + patente +  ", de prioridad " << prioridad << ". Se le cargaron " << capacidadTanque << " litros." << "\033[0m" << endl;
}

Auto::~Auto() {}

std::string Auto::getPatente() const{
	return patente;
}

long Auto::getPrioridad() const{
	return prioridad;
}

void Auto::setPrioridad(long nueva) {
	prioridad = nueva;
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

long Auto::generarPrioridadRandom() const{
	return numeroAlAzar(PRIOR_MIN,PRIOR_MAX);
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
	serial += llen.str() + SEPARADOR;
	stringstream pri;
	pri << prioridad;
	serial += pri.str() ;
	return serial;
}

void Auto::deserializar(const std::string& serial){
	char* serie = (char*) malloc(LONG_SERIE);
	if(!serie){
		patente = "ERR-DS";
		capacidadTanque = 0;
		lleno = true;
		prioridad = 0;
		return;
	}
	strcpy(serie,serial.c_str());
	//se toman los campos de acuerdo al separador
	patente = strtok(serie,SEPARADOR);
	capacidadTanque = atoi(strtok(NULL,SEPARADOR));
	lleno = atoi(strtok(NULL,SEPARADOR));
	prioridad = atoi(strtok(NULL,SEPARADOR));
	free(serie);
}

Auto::auto_t Auto::obtenerEstructura() const{
	struct auto_t estructura;
	memset(&estructura, 0, sizeof(struct auto_t));
	estructura.capacidad = capacidadTanque;
	estructura.estaLleno = lleno;
	strcpy(estructura.patente_auto, patente.c_str());
	estructura.mtype = prioridad;
	return estructura;
}

void Auto::reconstruir(struct auto_t autito){
	capacidadTanque = autito.capacidad;
	lleno = autito.estaLleno;
	patente = autito.patente_auto;
	prioridad = autito.mtype;
}

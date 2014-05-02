/*
 * GeneradorAutos.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "GeneradorAutos.h"
#include "ManejoTiempos.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

GeneradorAutos::GeneradorAutos(float media, const PipeAutos& canal) {
	this->media = media;
	envios = canal;
}

GeneradorAutos::~GeneradorAutos() {
}

std::string generarMensaje(int numAuto, string patente){
	std::string mensaje = "Generador de Autos: Auto ";

	stringstream ss;
	ss << numAuto;//add number to the stream
	mensaje += ss.str();
	mensaje += " creado, patente: ";
	mensaje += patente;
	mensaje += "\n";
	return mensaje;
}

void GeneradorAutos::generar(){
	pid_t id = fork();
	if (id != 0)
		return;
	inicializarRandom();
	int numAuto = 0;
	while(numAuto<5){ //TODO: con la variable de la signal handler
		Auto autito;
		cout << generarMensaje(numAuto, autito.getPatente());
		envios.escribirAuto(autito);
		int tiempo = (int) tiempoAlAzarExponencial(media);
		usleep(tiempo);
		numAuto++;
	}
	envios.cerrar();

	exit(0);
}




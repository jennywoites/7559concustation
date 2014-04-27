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

using namespace std;

GeneradorAutos::GeneradorAutos(float media) {
	this->media = media;
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

pid_t GeneradorAutos::generar(){
	pid_t id = fork();
	if (id != 0)
		return id;
	inicializarRandom();
	int numAuto = 0;
	while(true){ //TODO: con la variable de la signal handler
		Auto autito;
		cout << generarMensaje(numAuto, autito.getPatente());
		//Todo: mandar autito por pipe
		int tiempo = (int) tiempoAlAzarExponencial(media);
		usleep(tiempo);
		numAuto++;
	}

	return 0; //yo no soy el principal!
}




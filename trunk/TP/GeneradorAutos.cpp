/*
 * GeneradorAutos.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "GeneradorAutos.h"
#include "ManejoTiempos.h"
#include "Log.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "MecanismoConcurrencia/SIGINT_Handler.h"
#include "MecanismoConcurrencia/SignalHandler.h"
using namespace std;

GeneradorAutos::GeneradorAutos(float media, const PipeAutos& canal) {
	this->media = media;
	envios = canal;
}

GeneradorAutos::~GeneradorAutos() {
}

std::string generarMensaje(int numAuto, string patente){
	std::string mensaje = "FordMachine: Auto ";

	stringstream ss;
	ss << numAuto;//add number to the stream
	mensaje += ss.str();
	mensaje += " creado, patente: ";
	mensaje += patente;
	return mensaje;
}

pid_t GeneradorAutos::generar(){
	pid_t id = fork();
	if (id != 0)
		return id;

	inicializarRandom();

	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

	int numAuto = 0;

	Log::abrir_log();
	Log::enviarMensaje("Generador de Autos: llamenme Ford. Roque Ford");
	
	while (sigint_handler.getGracefulQuit() == 0 ) {
		Auto autito;
		Log::enviarMensaje(generarMensaje(numAuto, autito.getPatente()));
		envios.escribirAuto(autito);
		int tiempo = (int) tiempoAlAzarExponencial(media);
		usleep(tiempo);
		numAuto++;
	}
	Log::enviarMensaje("FordMachine: llego la crisis del 30, ya no hay mas trabajo por hacer");
	Log::enviarMensaje("FordMachine: Cierro el canal de autos");
	envios.cerrar();
	SignalHandler :: destruir ();
	cout << "Generador deja de generar y se cierra" << endl;
	
	Log::enviarMensaje("Fin de proceso Generador de Autos");
	exit(0);
}




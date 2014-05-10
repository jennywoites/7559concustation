/*
 * GeneradorAutos.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "GeneradorAutos.h"

#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>

#include "ManejoTiempos.h"

using namespace std;

GeneradorAutos::GeneradorAutos(float media, const PipeAutos& canal) {
	this->media = media;
	numAuto = 0;
	envios = canal;
}

GeneradorAutos::~GeneradorAutos(){}

void GeneradorAutos::comenzarDia(){
	inicializarRandom();

	envios.setearModo(Pipe::ESCRITURA);

	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

	log.setEscritor("Generador de Autos");
	log.escribirEntrada("llamenme Ford. Roque Ford");
	log.setEscritor("FordMachine");
}

void GeneradorAutos::finalizarDia(){
	log.escribirEntrada("Llego la crisis del 30, ya no hay mas trabajo por hacer");
	log.escribirEntrada("Cierro el canal de autos");
	envios.cerrar();
	SignalHandler :: destruir ();
	cout << "Generador deja de generar y se cierra" << endl;

	log.escribirEntrada("Fin de proceso Generador de Autos");
}

pid_t GeneradorAutos::generar(){
	pid_t id = fork();
	if (id != 0)
		return id;

	comenzarDia();
	
	while (sigint_handler.getGracefulQuit() == 0 ) {
		Auto autito;
		log.escribirEntrada("creado el auto:" + autito.getPatente() + ", numero " , numAuto);
		envios.escribirAuto(autito);
		int tiempo = (int) tiempoAlAzarExponencial(media);
		usleep(tiempo);
		numAuto++;
	}
	
	finalizarDia();
	exit(0);
}




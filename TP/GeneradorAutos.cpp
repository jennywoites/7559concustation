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
	log.setTipo(Log::ENTRADA_PERSONAJE);
}

GeneradorAutos::~GeneradorAutos(){}

bool GeneradorAutos::comenzarDia(){
	inicializarRandom();
	log.setEscritor("FordMachine");
	log.escribirEntrada("llamenme Ford. Roque Ford");

	try{
		envios.setearModo(Pipe::ESCRITURA);
	}catch (std::string &e){
		cout << e << endl;
		log.escribirEntrada("No pude setear modo de comunicacion Escritura");
		return false;
	}

	try{
		SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	}catch (std::string &e) {
		cout << e << endl;
		return false;
	}
	try{
		SignalHandler::getInstance()->registrarHandler(SIGPIPE, &sigpipe_handler);
	}catch (std::string &e) {
		cout << e << endl;
		return false;
	}

	return true;
}

void GeneradorAutos::cerrarPipe(){
	try{
		envios.cerrar();
		log.escribirEntrada("Me desadoso del pipe de generacion");
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("No se pudo desadosar de pipe de generacion");
	}
}

void GeneradorAutos::destruir(){
	cerrarPipe();
	SignalHandler :: destruir ();
}

void GeneradorAutos::finalizarDia(){
	log.escribirEntrada("Llego la crisis del 30, ya no hay mas trabajo por hacer");
	log.escribirEntrada("Cierro el canal de autos");
	destruir();
	cout << "Generador deja de generar y se cierra" << endl;

	log.escribirEntrada("Fin de proceso Generador de Autos");
}

pid_t GeneradorAutos::generar(){
	pid_t id = fork();
	if (id != 0)
		return id;

	bool comienzo = comenzarDia();
	if (!comienzo){
		destruir();
		log.escribirEntrada("Finalizo Proceso por ERROR.");
		return id;
	}
	
	while (sigint_handler.getGracefulQuit() == 0 and sigpipe_handler.getGracefulQuit() == 0 ) {
		Auto autito;
		log.escribirEntrada("creado el auto: " + autito.getPatente() + ", numero " , numAuto);
		//FIXME
		if(! envios.escribirAuto(autito)){
			log.escribirEntrada("se cerro mi lector inesperadamente");
			break;
		}
		int tiempo = (int) tiempoAlAzarExponencial(media);
		usleep(tiempo);
		numAuto++;
	}
	
	finalizarDia();

	return id;
}

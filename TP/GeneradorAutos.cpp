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
	}catch(const std::string &e){
		log.escribirEntrada("No pude setear modo de comunicacion Escritura." + e);
		return false;
	}

	try{
		SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
		log.escribirEntrada("Registro el manejo de finalizacion SIGINT");
	}catch(const std::string &e) {
		log.escribirEntrada("No se ha podido registrar el Signal Handler SIGINT." + e);
		return false;
	}
	try{
		SignalHandler::getInstance()->registrarHandler(SIGPIPE, &sigpipe_handler);
		log.escribirEntrada("Registro el manejo de finalizacion SIGPIPE");
	}catch(const std::string &e) {
		log.escribirEntrada("No se ha podido registrar el Signal Handler SIGPIPE." + e);
		return false;
	}

	return true;
}

void GeneradorAutos::cerrarPipe(){
	try{
		//cierra su Pipe de generacion
		envios.cerrar();
		log.escribirEntrada("Me desadoso del pipe de generacion");
	}catch(const std::string &e){
		log.escribirEntrada("No se pudo desadosar de pipe de generacion." + e);
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
	//ha ocurrido un error que no permite continuar con la ejecucion
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

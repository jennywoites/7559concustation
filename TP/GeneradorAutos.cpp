/*
 * GeneradorAutos.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "GeneradorAutos.h"

#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>

#include "ManejoTiempos.h"

using namespace std;

GeneradorAutos::GeneradorAutos(float media, const ColaAutos& canal):
	media(media),	numAuto(0),	envios(canal){
	log.setTipo(Log::ENTRADA_PERSONAJE);
}

GeneradorAutos::~GeneradorAutos(){}

bool GeneradorAutos::comenzarDia(){
	inicializarRandom();
	log.setEscritor("FordMachine");
	log.escribirEntrada("llamenme Ford. Roque Ford");

	try{
		SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
		log.escribirEntrada("Registro el manejo de finalizacion SIGINT");
	}catch(const std::string &e) {
		log.escribirEntrada("No se ha podido registrar el Signal Handler SIGINT." + e);
		return false;
	}
	try{
		//la signal SIGPIPE se genera por escribir sobre el Pipe cuando el Jefe tuvo inconvenientes
		SignalHandler::getInstance()->registrarHandler(SIGPIPE, &sigpipe_handler);
		log.escribirEntrada("Registro el manejo de finalizacion SIGPIPE");
	}catch(const std::string &e) {
		log.escribirEntrada("No se ha podido registrar el Signal Handler SIGPIPE." + e);
		return false;
	}

	return true;
}

void GeneradorAutos::destruir(){
	SignalHandler :: destruir ();
}

void GeneradorAutos::finalizarDia(){
	log.escribirEntrada("Llego la crisis del 30, ya no hay mas trabajo por hacer");

	Auto autito;
	autito.setPrioridad(Auto::PRIOR_MAX + 1);
	envios.escribirAuto(autito);


	log.escribirEntrada("Cierro el canal de autos");
	destruir();
	cout << "\033[1;31m" << "Generador deja de generar y se cierra" << "\033[0m" << endl;

	log.escribirEntrada("Fin de proceso Generador de Autos");
}

std::string GeneradorAutos::mensajeCreacion(const Auto& autito){
	std::string mensaje  = "Se ha generado el auto de patente ";
	mensaje += autito.getPatente() + " y prioridad ";
	stringstream ss;
	ss << autito.getPrioridad();
	mensaje += ss.str();
	mensaje += ".";
	return mensaje;
}

pid_t GeneradorAutos::generar(){
	pid_t id = fork();
	if (id != 0)
		return id;

	//Hijo: Generador de Autos

	bool comienzo = comenzarDia();
	//ha ocurrido un error que no permite continuar con la ejecucion
	if (!comienzo){
		destruir();
		log.escribirEntrada("Finalizo Proceso por ERROR.");
		return id;
	}
	
	//manejo de finalizacion: signals SIGINT y SIGPIPE
	while (sigint_handler.getGracefulQuit() == 0 and sigpipe_handler.getGracefulQuit() == 0 ) {
		//crea un auto aleatorio
		Auto autito;
		std::string mensaje = mensajeCreacion(autito);
		log.escribirEntrada(mensaje + " Numero: ", numAuto);
		cout << "\033[1;31m" << mensaje << "\033[0m" << endl;

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

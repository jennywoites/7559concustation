/*
 * Administrador.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "Administrador.h"

#include <stdlib.h>
#include <cmath>
#include <sstream>

#include "ManejoTiempos.h"

Administrador::Administrador(float m) {
	plataVista = 0;
	media = m;
	log.setTipo(Log::ENTRADA_PERSONAJE);
}

Administrador::~Administrador() {
}

bool Administrador::comenzarDia(){
	log.setEscritor("Administrador");

	log.escribirEntrada("Abro el log");

	bool abriCaja = caja.abrir();
	if(!abriCaja){
		log.escribirEntrada("No pude abrir la caja.");
		return false;
	}
	log.escribirEntrada("Abro la caja");
	cout << "Administrador abre la caja." << endl;

	try{
		SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	}catch (std::string &e) {
		cout << e << endl;
		return false;
	}

	log.escribirEntrada("Registro el manejo de finalizacion");
	return true;
}

void Administrador::destruir(){
	bool cerre = caja.cerrar();
	if(!cerre)
		log.escribirEntrada("No pude cerrar la caja.");
	else
		log.escribirEntrada("Cerre la caja");

	SignalHandler::destruir ();
}

void Administrador::finalizarDia(){
	log.escribirEntrada("Recibe signal de finalizar funcionamiento.");
	destruir();
	cout << "Administrador se va a su casa." << endl;
	log.escribirEntrada("Dejo de revisar caja, cierro Proceso correctamente.");
}
void Administrador::pensar(){
	float espera = tiempoAlAzarExponencial(media);
	int tiempo_entero = floor(espera);
	log.escribirEntrada("Pienso antes de ir a la caja durante: ", tiempo_entero);
	usleep(tiempo_entero);
}

void Administrador::imprimirDinero(){
	stringstream ss;
	ss << plataVista;
	cout << "El administrador fue a la caja, vio " + ss.str()  << endl;
}

void Administrador::mirarDinero(){
	log.escribirEntrada("Voy a la caja");
	float plata_actual = caja.verMonto();
	if (plataVista != plata_actual){
		plataVista = plata_actual;
		log.escribirEntrada("Hay mas plata! ahora hay $", plata_actual);
	}else{
		log.escribirEntrada("Hay la misma cantidad de plata :( hay $", plata_actual);
	}
	imprimirDinero();
}

pid_t Administrador::administrarCaja(){
	pid_t id = fork();
	if (id != 0)
		return id;

	bool comienzo = comenzarDia();
	if(!comienzo){
		destruir();
		log.escribirEntrada("Finalizo Proceso por ERROR.");
		return id;
	}

	while (sigint_handler.getGracefulQuit() == 0 ) {
		pensar();

		mirarDinero();
	}

	finalizarDia();

	return id;
}


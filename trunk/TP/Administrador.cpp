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

Administrador::Administrador(float m): plataVista(0), media(m) {
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
	cout << "\033[1;33m" << "Administrador abre a la caja" << "\033[0m" << endl;

	try{
		SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
		log.escribirEntrada("Registro el manejo de finalizacion SIGINT");
	}catch (std::string &e) {
		log.escribirEntrada("No se ha podido registrar el Signal Handler." + e);
		return false;
	}
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
	cout << "\033[1;33m" << "Administrador se va a su casa." << "\033[0m" << endl;
	log.escribirEntrada("Dejo de revisar caja, cierro Proceso correctamente.");
	caja.eliminar();
}
void Administrador::pensar(){
	//espera intervalos de tiempo para revisar la caja
	float espera = tiempoAlAzarExponencial(media);
	int tiempo_entero = floor(espera);
	log.escribirEntrada("Pienso antes de ir a la caja durante: ", tiempo_entero);
	usleep(tiempo_entero);
}

void Administrador::imprimirDinero(){
	stringstream ss;
	ss << plataVista;
	cout << "\033[1;33m" << "El administrador fue a la caja, vio $" << ss.str() << "\033[0m" << endl;
}

void Administrador::mirarDinero(){
	log.escribirEntrada("Voy a la caja");
	caja.lectorQuiereUsar();
	float plata_actual = caja.verMonto();
	caja.lectorDeja();
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
	//Hijo: Administrador

	bool comienzo = comenzarDia();
	//ha ocurrido un error que no permite continuar con la ejecucion
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


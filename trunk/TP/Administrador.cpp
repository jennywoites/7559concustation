/*
 * Administrador.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "Administrador.h"

#include <stdlib.h>
#include <cmath>

#include "ManejoTiempos.h"

Administrador::Administrador(float m) {
	plata_anterior = 0;
	media = m;
	log.setTipo(Log::ENTRADA_PERSONAJE);
}

Administrador::~Administrador() {
}

void Administrador::comenzarDia(){
	log.setEscritor("Administrador");

	log.escribirEntrada("Abro el log");

	caja.abrir();
	log.escribirEntrada("Abro la caja");

	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	log.escribirEntrada("Registro el manejo de finalizacion");
}

void Administrador::finalizarDia(){
	log.escribirEntrada("Recibe signal de finalizar funcionamiento.");
	caja.cerrar();
	log.escribirEntrada("Cerre la caja");
	SignalHandler::destruir ();
	log.escribirEntrada("Administrador deja de revisar caja y se cierra");
}
void Administrador::pensar(){
	float espera = tiempoAlAzarExponencial(media);
	int tiempo_entero = floor(espera);
	log.escribirEntrada("Pienso antes de ir a la caja durante: ", tiempo_entero);
	usleep(tiempo_entero);
}

void Administrador::mirarDinero(){
	log.escribirEntrada("Voy a la caja");
	float plata_actual = caja.verMonto();
	if (plata_anterior != plata_actual){
		plata_anterior = plata_actual;
		log.escribirEntrada("Hay mas plata! ahora hay $", plata_actual);
	}else{
		log.escribirEntrada("Hay la misma cantidad de plata :( hay $", plata_actual);
	}
	cout << "El administrador fue a la caja." << endl;
}

pid_t Administrador::administrarCaja(){
	pid_t id = fork();
	if (id != 0)
		return id;

	comenzarDia();

	while (sigint_handler.getGracefulQuit() == 0 ) {
		pensar();

		mirarDinero();
	}

	finalizarDia();

	return id;
}


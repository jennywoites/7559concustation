/*
 * Administrador.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "Administrador.h"

#include <unistd.h>
#include <stdlib.h>
#include <cmath>

#include "MecanismoConcurrencia/SIGINT_Handler.h"
#include "MecanismoConcurrencia/SignalHandler.h"
#include "ManejoTiempos.h"
#include "Log.h"

Administrador::Administrador(float m) {
	plata_anterior = 0;
	media = m;
}

Administrador::~Administrador() {

}

void Administrador::mirarDinero(){
	Log::enviarMensaje("Voy a la caja");
	float plata_actual = caja.verMonto();
	if (plata_anterior != plata_actual){
		plata_anterior = plata_actual;
		Log::enviarMensaje("Hay mas plata! ahora hay $", plata_actual);
	}else{
		Log::enviarMensaje("Hay la misma cantidad de plata :( hay $", plata_actual);
	}
	cout << "El administrador fue a la caja." << endl;
}

pid_t Administrador::administrarCaja(){
	pid_t id = fork();
	if (id != 0)
		return id;

	Log::abrir_log();
	Log::setEscritor("Administrador");
	Log::enviarMensaje("Abro el log.");

	caja.abrir();
	Log::enviarMensaje("Abro la caja");

	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

	while (sigint_handler.getGracefulQuit() == 0 ) {
		float espera = tiempoAlAzarExponencial(media);
		int tiempo_entero = floor(espera);
		Log::enviarMensaje("Me duermo antes de ir a la caja durante: ",tiempo_entero);
		usleep(tiempo_entero);
		mirarDinero();
	}

	Log::enviarMensaje("Recibe signal de finalizar funcionamiento.");
	caja.cerrar();
	Log::enviarMensaje("Cerre la caja");
	SignalHandler :: destruir ();
	Log::enviarMensaje("Administrador deja de revisar caja y se cierra");
	Log::cerrar_log();
	exit(0);
}


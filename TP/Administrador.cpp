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

void Administrador::printDebug(std::string msj){
	Log::enviarMensaje("Administrador: " + msj);
}

void Administrador::printDebug(std::string msj, int numero){
	Log::enviarMensaje("Administrador: " + msj, numero);
}

Administrador::~Administrador() {

}

pid_t Administrador::mirarDinero(float inicial){
	pid_t id = fork();
	if (id != 0)
		return id;

	Log::abrir_log();
	printDebug("Soy un nuevo proceso, con pid: ", getpid());
	printDebug("Abro el log.");

	caja.abrir(inicial);
	plata_anterior = inicial;
	printDebug("Abro caja con un monto inicial de $",inicial);

	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

	int i = 10;
	while (sigint_handler.getGracefulQuit() == 0 ) {
		float espera = tiempoAlAzarExponencial(media);
		int tiempo_entero = floor(espera);
		printDebug("Me duermo antes de ir a la caja durante: ",tiempo_entero);
		usleep(tiempo_entero);

		printDebug("Voy a la caja");
		float plata_actual = caja.verMonto();
		if (plata_anterior != plata_actual){
			plata_anterior = plata_actual;
			printDebug("Hay mas plata! ahora hay $", plata_actual);
		}else{
			printDebug("Hay la misma cantidad de plata :( hay $", plata_actual);
		}
		cout << "El administrador fue a la caja." << endl;
		i--;
	}

	printDebug("Recibe signal de finalizar funcionamiento.");
	caja.cerrar();
	printDebug("Cerre la caja");
	SignalHandler :: destruir ();
	Log::enviarMensaje("Administrador deja de revisar caja y se cierra");
	Log::cerrar_log();
	exit(0);
}


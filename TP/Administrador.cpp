/*
 * Administrador.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "Administrador.h"
#include "ManejoTiempos.h"
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include "Log.h"

Administrador::Administrador(float m) {
	plata_anterior = 0;
	media = m;
}

Administrador::~Administrador() {

}

void Administrador::mirarDinero(float inicial){
	if (fork() != 0)
		return;

	Log::abrir_log();
	Log::enviarMensaje("Administrador abre el log");

	caja.abrir(inicial);
	plata_anterior = inicial;

	int i = 10;
	while (i > 0){ //envio de señal
		float espera = tiempoAlAzarExponencial(media);
		int tiempo_entero = floor(espera);
		usleep(tiempo_entero);

		Log::enviarMensaje("Administrador: voy a la caja");
		float plata_actual = caja.verMonto();
		stringstream ss;
		ss<<plata_actual;
		if (plata_anterior != plata_actual){
			plata_anterior = plata_actual;
			Log::enviarMensaje("Administrador: Hay mas plata! ahora hay $" + ss.str());
		}else{
			Log::enviarMensaje("Administrador: Hay la misma cantidad de plata :( hay $"+ ss.str());
		}
		cout << "El administrador fue a la caja \n";
		i--;
	}
	caja.cerrar();
	Log::cerrar_log();
	exit(0);
}


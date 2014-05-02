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
#include "Log.h"

Administrador::Administrador(float m) {
	plata_anterior = 0;
	media = m;
	//caja = Caja();
}

Administrador::~Administrador() {

}

void Administrador::mirarDinero(){
	if (fork() != 0)
		return;

	caja.abrir();
	Log::abrir_log();

	Log::enviarMensaje("Administrador abre el log");

	int i = 10;
	while (i > 0){ //envio de señal
		float espera = tiempoAlAzarExponencial(media);
		int tiempo_entero = floor(espera);
		usleep(tiempo_entero);

		Log::enviarMensaje("Administrador: voy a la caja");
		float plata_actual = caja.verMonto();
		if (plata_anterior != plata_actual){
			plata_anterior = plata_actual;
			Log::enviarMensaje("Administrador: Hay mas plata!");
		}else{
			Log::enviarMensaje("Administrador: Hay la misma cantidad de plata :(");
		}
		cout << "Administrador fue a la caja \n";
		i--;
	}
	caja.cerrar();
	exit(0);
}


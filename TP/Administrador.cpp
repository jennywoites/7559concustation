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

Administrador::Administrador(float m) {
	plata_anterior = 0;
	media = m;
}

Administrador::~Administrador() {

}

void Administrador::mirarDinero(){
	if (fork() != 0)
		return;

	int i = 0;
	while (i < 10){ //envio de señal
		float espera = tiempoAlAzarExponencial(media);
		int tiempo_entero = floor(espera);
		usleep(tiempo_entero);

		cout << "Administrador: voy a la caja\n"; //a cout o log, despues lo vemos
		float plata_actual = caja.verMonto();
		if (plata_anterior != plata_actual){
			plata_anterior = plata_actual;
			cout << "Administrador: Hay mas plata!\n";
		}else{
			cout << "Administrador: Hay la misma cantidad de plata :(\n";
		}
		i++;
	}
	caja.cerrar();
	exit(0);
}


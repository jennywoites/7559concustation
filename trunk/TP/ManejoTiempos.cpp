/*
 * ManejoTiempos.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include "ManejoTiempos.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

void inicializarRandom(){
	srand ( time(NULL) );
}

int numeroAlAzar(int min , int max){
	int longitud = max - min;
	int resul = min + rand() % (longitud + 1);
	return resul;
}

float tiempoAlAzarExponencial(float media){
	float u;
	do{
		u = rand() % 1000;
		u /= 1000;
	}while(u == 1);

	float rta = (-media) * log (1-u);
	return rta;
}

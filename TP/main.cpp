/*
 * main.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include <stdio.h>
#include "Auto.h"
#include "ManejoTiempos.h"

int main(void){
	inicializarRandom();
	printf("Número al azar del 1 al 10: %d\n", numeroAlAzar(1,10));
	printf("Valor de variable aleatoria exponencial de media 1: %f\n", tiempoAlAzarExponencial(1));

	Auto fitito;
	printf("Patente aleatoria de un auto: %s\n", fitito.getPatente().c_str());
	printf("Le llené %d litros al auto\n", fitito.llenar());
	return 0;
}



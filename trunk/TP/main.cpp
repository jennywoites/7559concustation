/*
 * main.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include <stdio.h>
#include <iostream>
#include "Auto.h"
#include "Jefe.h"
#include "ManejoTiempos.h"
#include "GeneradorAutos.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void){
	inicializarRandom();
	GeneradorAutos generator(3000000);
	generator.generar();

	Jefe jefe("Martinez");
	jefe.atenderAutos();

	wait(NULL);
	exit(0);
}



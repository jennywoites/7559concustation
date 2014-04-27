/*
 * main.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include <stdio.h>
#include "Auto.h"
#include "ManejoTiempos.h"
#include "GeneradorAutos.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void){
	inicializarRandom();
	GeneradorAutos generator(15000000);
	pid_t id = generator.generar();
	if (id == 0) exit (0);
	wait(NULL);
	exit(0);
}



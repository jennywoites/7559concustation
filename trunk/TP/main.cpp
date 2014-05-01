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
#include "Caja.h"
#include "Administrador.h"


int main(void){
	Caja caja(0);
	Administrador admin(10000);
	admin.mirarDinero();
	wait(NULL);
	cout <<"Cierro la caja\n";
	exit(0);
}



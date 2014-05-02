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
#include "MecanismoConcurrencia/PipeAutos.h"

void prueba1(void){
	Caja caja(0);
	Administrador admin(10000);
	admin.mirarDinero();
	wait(NULL);
	cout <<"Cierro la caja\n";
	exit(0);
}

void pruebaPipeAutos(void){
	PipeAutos pipe;
	if (fork()==0){
		Auto a("ABC123", 10);
		pipe.escribirAuto(a);
	}else{
		Auto b;
		cout << pipe.leerAuto(&b) << endl;
		cout << b.getPatente() << endl;
	}
}

void pruebaGenPipeAutos(void){
	PipeAutos generacion;
	Jefe j ("UltraAlterMaster", generacion);
	j.atenderAutos();
	GeneradorAutos g (100000, generacion);
	g.generar();
}

int main(void){
	//prueba1();
	//pruebaPipeAutos();
	pruebaGenPipeAutos();
}

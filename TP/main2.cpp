/*
 * main2.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "Caja.h"
#include "Administrador.h"
#include "Auto.h"
#include "Jefe.h"
#include "EstacionDeServicio.h"
#include "ManejoTiempos.h"
#include "GeneradorAutos.h"

#include <getopt.h>
#define SURTIDORES 1
#define EMPLEADOS 1
#define MEDIA_AUTOS 100
#define MEDIA_ADMIN 100
#define OPC_ERROR -1
#define OPC_IMPRIMIR_AYUDA 0
#define OPC_IMPRIMIR_VERSION 1
#define OPC_EXEC 2

void imprimir_ayuda(){
	cout << "OPCIONES:" << endl;
	cout << "-h --help  Imprime en pantalla informacion de Ayuda." << endl;
	cout << "-v --version  Imprime la versión del programa." << endl;
	cout << "-s --surtidores  Define cantidad de surtidores." << endl;
	cout << "-e --empleados  Define cantidad de empleados." << endl;
	cout << "-g --genautos  Define media de generacion de autos." << endl;
	cout << "-a --veradmin  Define media de visita de administrador a la caja." << endl;
}

//imprime la version del programa al stdout
void imprimir_version(){
	cout << "Primer Proyecto - 75.59 - TPCI" << endl;
	cout << "ConcuStation" << endl;
	cout << "Primer Cuatrimestre - 2014" << endl;
	cout << "Grupo:" << endl;
	cout << "BUCHWALD, Martin Ezequiel (93155)" << endl;
	cout << "GENENDER PEÑA, Ezequiel David (93163)" << endl;
	cout << "WOITES, Jennifer Andrea (93274)" << endl;
}


int parsearParametros(char* argv[], int argc, int* cantSurtidores, int* cantEmpleados, int* mediaAutos, int* mediaAdmin){
	*cantSurtidores = SURTIDORES;
	*cantEmpleados = EMPLEADOS;
	*mediaAutos = MEDIA_AUTOS;
	*mediaAdmin = MEDIA_ADMIN;

	//struct de lineas de comando
	struct option opciones[]={
		{"help",no_argument,NULL,'h'},
		{"version",no_argument,NULL,'v'},
		{"surtidores",required_argument,NULL,'s'},
		{"empleados",required_argument,NULL,'e'},
		{"genautos",required_argument,NULL,'g'},
		{0,0,0,0}
	};

	char caracter;
	//mientras haya opciones las lee y las procesa
	while ((caracter = (getopt_long(argc,argv,"hvs:e:g:a:",opciones,NULL)))!=-1){
		switch(caracter){
			case 'h'://help
				return OPC_IMPRIMIR_AYUDA;
			case 'v'://Version
				return OPC_IMPRIMIR_VERSION;
			case 's':
				if (strcmp(optarg,"-")!=0)
					*cantSurtidores = atoi(optarg);
				break;
			case 'e':
				if (strcmp(optarg,"-")!=0)
					*cantEmpleados = atoi(optarg);
				break;
			case 'g':
				if (strcmp(optarg,"-")!=0)
					*mediaAutos = atoi(optarg);
					break;
			case 'a':
				if (strcmp(optarg,"-")!=0)
					*mediaAdmin = atoi(optarg);
				break;
			case '?'://error
				return OPC_ERROR;
		}
	}
	return OPC_EXEC;
}

void atender(int cantSurtidores, int cantEmpleados, int mediaAutos, int mediaAdmin){
	EstacionDeServicio concuStation (cantEmpleados, cantSurtidores, mediaAutos, mediaAdmin);
	concuStation.abrir();
	sleep(10000); //FIXME: tiempo de simu
	concuStation.cerrar();
}

int main2(char* argv[], int argc){
	int cantSurtidores, cantEmpleados, mediaAutos, mediaAdmin;

	int opcion = parsearParametros(argv, argc, &cantSurtidores, &cantEmpleados, &mediaAutos, &mediaAdmin);

	switch (opcion){
		case OPC_EXEC:
			atender(cantSurtidores, cantEmpleados, mediaAutos, mediaAdmin);
			break;
		case OPC_IMPRIMIR_AYUDA:
			imprimir_ayuda();
			break;
		case OPC_IMPRIMIR_VERSION:
			imprimir_version();
			break;
		case OPC_ERROR:
			cout << "Parametros no validos. Intente nuevamente" << endl;
			break;
	}

	exit(0);
}

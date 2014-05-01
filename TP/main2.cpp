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
#define MONTO_INICIAL 0
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
	cout << "-m --monto  Define monto inicial." << endl;
}

//imprime la version del programa al stdout
void imprimir_version(){
	cout << "Trabajo Practico - TPCI" << endl;
	cout << "ConcuStation" << endl;
	cout << "Primer Cuatrimestre - 2014" << endl;
	cout << "Grupo:" << endl;
	cout << "BUCHWALD, Martin Ezequiel (93155)" << endl;
	cout << "GENENDER PEÑA, Ezequiel David (93163)" << endl;
	cout << "WOITES, Jennifer Andrea (93274)" << endl;
}


int parsearParametros(char* argv[], int argc, int* cantSurtidores, int* cantEmpleados, int* mediaAutos, float* montoInicial){
	*cantSurtidores = SURTIDORES;
	*cantEmpleados = EMPLEADOS;
	*mediaAutos = MEDIA_AUTOS;
	*montoInicial = MONTO_INICIAL;

	//struct de lineas de comando
	struct option opciones[]={
		{"help",no_argument,NULL,'h'},
		{"version",no_argument,NULL,'v'},
		{"surtidores",no_argument,NULL,'s'},
		{"empleados",no_argument,NULL,'e'},
		{"genautos",no_argument,NULL,'g'},
		{"monto",required_argument,NULL,'m'},
		{0,0,0,0}
	};

	char caracter;
	//mientras haya opciones las lee y las procesa
	while ((caracter = (getopt_long(argc,argv,"hvs:e:g:m:",opciones,NULL)))!=-1){
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
			case 'm':
				if (strcmp(optarg,"-")!=0)
					*montoInicial = atof(optarg);
				break;
			case '?'://error
				return OPC_ERROR;
		}
	}
	return OPC_EXEC;
}

void atender(int cantSurtidores, int cantEmpleados, int mediaAutos, float montoInicial){
	EstacionDeServicio concuStation (cantEmpleados, cantSurtidores, mediaAutos);
	concuStation.abrir(montoInicial);
	sleep(10000); //FIXME: tiempo de simu
	concuStation.cerrar();
}

int main2(char* argv[], int argc){
	int cantSurtidores, cantEmpleados, mediaAutos;
	float montoInicial;

	int opcion = parsearParametros(argv, argc, &cantSurtidores, &cantEmpleados, &mediaAutos, &montoInicial);

	switch (opcion){
		case OPC_EXEC:
			atender(cantSurtidores, cantEmpleados, mediaAutos, montoInicial);
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

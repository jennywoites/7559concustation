/*
 * main.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "EstacionDeServicio.h"

#include <getopt.h>
#define SURTIDORES 1
#define EMPLEADOS 1
#define MEDIA_AUTOS 100000
#define MEDIA_ADMIN 250000
#define TIEMPO_SIMULACION 3
#define TIEMPO_ESPERA_EN_CAJA 1
#define OPC_ERROR -1
#define OPC_ERROR_SURTI -2
#define OPC_ERROR_TIEMPO -3
#define OPC_ERROR_NUM -4
#define OPC_IMPRIMIR_AYUDA 0
#define OPC_IMPRIMIR_VERSION 1
#define OPC_EXEC 2

//imprime la ayuda del programa al stdout
void imprimir_ayuda(){
	cout << "OPCIONES:" << endl;
	cout << "-h --help  Imprime en pantalla informacion de Ayuda." << endl;
	cout << "-v --version  Imprime la versión del programa." << endl;
	cout << "-s --surtidores  Define cantidad de surtidores." << endl;
	cout << "-e --empleados  Define cantidad de empleados." << endl;
	cout << "-g --genautos  Define media de generacion de autos." << endl;
	cout << "-a --veradmin  Define media de visita de administrador a la caja." << endl;
	cout << "-t --tiempo Define el tiempo que la estacion se econtrara abierta." << endl;
	cout << "-c --tiempoCaja Define el tiempo que demoran en depositar el dinero en la caja." << endl;
}

//imprime la version del programa al stdout
void imprimir_version(){
	cout << "Segundo Proyecto - 75.59 - TPCI" << endl;
	cout << "ConcuStation" << endl;
	cout << "Primer Cuatrimestre - 2014" << endl;
	cout << "Grupo:" << endl;
	cout << "BUCHWALD, Martin Ezequiel (93155)" << endl;
	cout << "GENENDER PEÑA, Ezequiel David (93163)" << endl;
	cout << "WOITES, Jennifer Andrea (93274)" << endl;
}


int parsearParametros(char* argv[], int argc, int* cantSurtidores, int* cantEmpleados, int* mediaAutos, int* mediaAdmin, int* tiempo, int * tiempoCaja){
	*cantSurtidores = SURTIDORES;
	*cantEmpleados = EMPLEADOS;
	*mediaAutos = MEDIA_AUTOS;
	*mediaAdmin = MEDIA_ADMIN;
	*tiempo = TIEMPO_SIMULACION;
	*tiempoCaja = TIEMPO_ESPERA_EN_CAJA;

	//struct de lineas de comando
	struct option opciones[]={
		{"help",no_argument,NULL,'h'},
		{"version",no_argument,NULL,'v'},
		{"surtidores",required_argument,NULL,'s'},
		{"empleados",required_argument,NULL,'e'},
		{"genautos",required_argument,NULL,'g'},
		{"veradmin",required_argument,NULL,'a'},
		{"tiempo",required_argument,NULL,'t'},
		{"tiempoCaja",required_argument,NULL,'c'},
		{0,0,0,0}
	};

	char caracter;
	//mientras haya opciones las lee y las procesa
	while ((caracter = (getopt_long(argc,argv,"hvs:e:g:a:t:c:",opciones,NULL)))!=-1){
		switch(caracter){
			case 'h'://help
				return OPC_IMPRIMIR_AYUDA;
			case 'v'://Version
				return OPC_IMPRIMIR_VERSION;
			case 's':
				if (strcmp(optarg,"-")!=0){
					*cantSurtidores = atoi(optarg);
					if(*cantSurtidores <= 0 or *cantSurtidores>256)
						return OPC_ERROR_SURTI;
				}
				break;
			case 'e':
				if (strcmp(optarg,"-")!=0){
					*cantEmpleados = atoi(optarg);
					if(*cantEmpleados < 0)
						return OPC_ERROR_NUM;
				}
				break;
			case 'g':
				if (strcmp(optarg,"-")!=0){
					*mediaAutos = atoi(optarg);
					if(*mediaAutos <= 0)
						return OPC_ERROR_NUM;
				}
				break;
			case 'a':
				if (strcmp(optarg,"-")!=0){
					*mediaAdmin = atoi(optarg);
					if(*mediaAdmin <= 0)
						return OPC_ERROR_NUM;
				}
				break;
			case 't':
				if (strcmp(optarg,"-")!=0){
					*tiempo = atoi(optarg);
					if(*tiempo <= 0)
						return OPC_ERROR_TIEMPO;
				}
				break;
			case 'c':
				if (strcmp(optarg,"-")!=0){
					*tiempoCaja = atoi(optarg);
					if(*tiempoCaja < 0)
						return OPC_ERROR_TIEMPO;
				}
				break;
			case '?'://error
				return OPC_ERROR;
		}
	}
	return OPC_EXEC;
}

void atender(int cantSurtidores, int cantEmpleados, int mediaAutos, int mediaAdmin, int tiempo, int tiempoCaja){
	EstacionDeServicio concuStation (cantEmpleados, cantSurtidores, mediaAutos, mediaAdmin);
	int estadoApertura = concuStation.abrir(tiempoCaja);

	if(estadoApertura == EstacionDeServicio::SOY_HIJO) return;

	if(estadoApertura == EstacionDeServicio::ERROR){
		cout << "Error al abrir Estacion de Servicio. FIN DEL PROGRAMA" <<endl;
		return;
	}

	//simula la duracion del dia
	sleep(tiempo);
	concuStation.cerrar();
}

int main(int argc, char* argv[]){
	int cantSurtidores, cantEmpleados, mediaAutos, mediaAdmin,tiempo, tiempoCaja;

	int opcion = parsearParametros(argv, argc, &cantSurtidores, &cantEmpleados, &mediaAutos, &mediaAdmin, &tiempo, &tiempoCaja);

	switch (opcion){
		case OPC_EXEC:	//caso ejecucion del proyecto
			try{
				atender(cantSurtidores, cantEmpleados, mediaAutos, mediaAdmin,tiempo,tiempoCaja);
			}catch(const std::string &e){
				cerr << "No pudo abrirse el Log: " + e << endl;
			}
			break;
		case OPC_IMPRIMIR_AYUDA:	//caso ayuda
			imprimir_ayuda();
			break;
		case OPC_IMPRIMIR_VERSION:	//caso version
			imprimir_version();
			break;
		case OPC_ERROR:
			cout << "Parametros no validos. Intente nuevamente" << endl;
			break;
		case OPC_ERROR_SURTI:
			cout << "Cantidad de Surtidores no valida, debe estar entre 1 y 256" << endl;
			break;
		case OPC_ERROR_TIEMPO:
			cout << "Tiempo no valido, debe ser mayor a 0" << endl;
			break;
		case OPC_ERROR_NUM:
			cout << "Los parametros deben ser numericos" << endl;
			break;
	}

	exit(0);
}

/*
 * EstacionDeServicio.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "EstacionDeServicio.h"

#include <signal.h>
#include <sys/wait.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "constantesArchivos.h"

#define ARCH_NOMBRES "TP/nombres.jem"

EstacionDeServicio::EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos, int mediaAdmin){
	cantEmpleados = empleados;
	cantSurtidores = surtidores;
	mediaAutos = mediaGenAutos;
	mediaVerAdmin = mediaAdmin;
	pidGen = 0;
	pidAdmin = 0;
	log.setTipo(Log::ENTRADA_PERSONAJE);
}

EstacionDeServicio::~EstacionDeServicio() {}

void EstacionDeServicio::agregarNombres(vector<std::string>& nombres){
	//FIXME: Usar bien el archivo para obtener los nombres, verificar si existe

    ifstream archNombres(ARCH_NOMBRES);

    char* name;
    string linea;

    while (getline(archNombres, linea)) {
    	name = new char [linea.size() + 1];
    	strcpy(name, linea.c_str());
    	cout << linea << endl;
    	nombres.push_back(name);
    	delete name;
    }

    archNombres.close();

}

std::string EstacionDeServicio::obtenerNombre(vector<std::string>& nombres, int i){
	std::string nombre;
	int largo = nombres.size();
	if(largo == 0){
		stringstream ss;
		ss << i;
		nombre = ss.str();
	}else{
		nombre = nombres.at(i%largo);
	}
	return nombre;
}

bool EstacionDeServicio::crearEmpleados(const PipeAutos& pipe, const Semaforo& surtidores){
	vector<std::string> nombres;
	agregarNombres(nombres);
	for(int i = 0; i<cantEmpleados; i++){
		std::string nombre = obtenerNombre(nombres,i);
		Empleado e (nombre, pipe, surtidores);
		log.escribirEntrada("Cree el empleado " + nombre);
		pid_t pidEmpleado = e.atenderAutos(cantSurtidores);
		if (pidEmpleado == 0) //como soy hijo, lo indico con true
			return true;
	}
	return false; //estacion crea todos los empleados, es padre
}

void EstacionDeServicio::crearAdmin(){
	Administrador a (mediaVerAdmin);
	pidAdmin = a.administrarCaja();
	log.escribirEntrada("Creo mi administrador");
}

pid_t EstacionDeServicio::crearJefe(){
	Jefe j ("UltraAlterMaster", generacion, atencion);
	log.escribirEntrada("Cree el jefe.");
	pid_t pidJefe = j.atenderAutos();
	log.escribirEntrada("Envie al jefe a atender autos.");
	return pidJefe;
}

void EstacionDeServicio::crearGenerador(){
	GeneradorAutos g (mediaAutos, generacion);
	log.escribirEntrada("Creo el generador de autos");
	pidGen = g.generar();
	log.escribirEntrada("Envie al generador a generar autos.");
}

void EstacionDeServicio::abrir(){
	Log::setModo(Log::MODO_DEBUG);
	log.mensajeApertura();
	log.setEscritor("Estacion de Servicio");
	log.escribirEntrada("Comienzo a funcionar. ABIERTO");

	crearAdmin();
	if(pidAdmin == 0) //Soy el proceso hijo que volvio
		return ;

	atencion.crear(ARCHIVO_ATENCION);
	log.escribirEntrada("Creo el pipe de atencion de autos.");

	//Creo los empleados, el generador, el administrador y el jefe y los mando
	// a que atiendan. Cada uno crea sus procesos correspondientes
	surtidores.crear(ARCHIVO_ACCESO_SURTIDORES,ACCESO_SURTIDORES,cantSurtidores); //Semaforo de valor M= cantidad de surtidores

	bool soyHijo = crearEmpleados(atencion,surtidores);
	if (soyHijo)
		return ;

	log.escribirEntrada("Termine de crear todos los empleados.");


	generacion.crear(ARCHIVO_GENERACION);
	log.escribirEntrada("Creo el pipe de generacion de autos.");

	pid_t pidJefe = crearJefe();
	if(pidJefe == 0) //Soy el proceso hijo que volvio
		return ;

	//Cierro pipe de atencion
	atencion.cerrar();
	log.escribirEntrada("Me desadoso del pipe de atencion.");

	crearGenerador();
	if(pidGen == 0) //Soy el proceso hijo que volvio
		return ;

	//Cierro pipe de generacion
	generacion.cerrar();
	log.escribirEntrada("Me desadoso del pipe de generacion de autos.");
}


void EstacionDeServicio::esperarCierre(){
	int i = cantEmpleados + 1 + 1; //Jefe + Generador de Autos
	while (i > 0) {
		wait(NULL);
		i--;
	}
}

void EstacionDeServicio::cerrar(){
	log.escribirEntrada("Comienzo CIERRE de estacion de servicio.");

	kill(pidGen, SIGINT);	// envia senial a generador
	log.escribirEntrada("Envie signal de finalizar al generador");

	esperarCierre();	// esperar cierre

	atencion.liberar();
	generacion.liberar();
	surtidores.eliminar();

	log.escribirEntrada("Espere a todos mis hijos: empleados, jefe y generador de autos.");

	kill(pidAdmin, SIGINT);	// envia senial a generador
	log.escribirEntrada("Envie signal de finalizar al administrador");
	wait(NULL); //admin

	log.escribirEntrada("Ya se cerraron todos los empleados, el jefe, el generador y el administrador");
	log.mensajeCierre();
}

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
#include <errno.h>
#include <string.h>

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

int EstacionDeServicio::abrir(){
	Log::setModo(Log::MODO_DEBUG);
	log.mensajeApertura();
	log.setEscritor("Estacion de Servicio");
	log.escribirEntrada("Comienzo a funcionar. ABIERTO");

	crearAdmin();
	if(pidAdmin == 0)
		return SOY_HIJO;

	try{
		atencion.crear(ARCHIVO_ATENCION);
		log.escribirEntrada("Creo el pipe de atencion de autos.");
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("No pude crear el pipe de atencion de autos.");
		finalizarAdministrador();
		log.escribirEntrada("Se finaliza el proceso por ERROR");
		log.mensajeCierre();
		return ERROR;
	}


	//Creo los empleados, el generador, el administrador y el jefe y los mando
	// a que atiendan. Cada uno crea sus procesos correspondientes
	surtidores.crear(ARCHIVO_ACCESO_SURTIDORES,ACCESO_SURTIDORES,cantSurtidores); //Semaforo de valor M= cantidad de surtidores

	bool soyHijo = crearEmpleados(atencion,surtidores);
	if (soyHijo)
		return SOY_HIJO;

	log.escribirEntrada("Termine de crear todos los empleados.");

	try{
		generacion.crear(ARCHIVO_GENERACION);
		log.escribirEntrada("Creo el pipe de generacion de autos.");
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("No pude crear el pipe de generacion de autos.");
		atencion.cerrar();
		esperarCierre();
		surtidores.eliminar();
		finalizarAdministrador();
		log.escribirEntrada("Se finaliza el proceso por ERROR");
		log.mensajeCierre();
		return ERROR;
	}



	pid_t pidJefe = crearJefe();
	if(pidJefe == 0)
		return SOY_HIJO;

	//Cierro pipe de atencion
	atencion.cerrar();
	log.escribirEntrada("Me desadoso del pipe de atencion.");

	crearGenerador();
	if(pidGen == 0)
		return SOY_HIJO;

	//Cierro pipe de generacion
	generacion.cerrar();
	log.escribirEntrada("Me desadoso del pipe de generacion de autos.");

	return OK;
}


void EstacionDeServicio::esperarCierre(){
	int i = cantEmpleados + 1 + 1; //Jefe + Generador de Autos
	while (i > 0) {
		wait(NULL);
		i--;
	}
}

void EstacionDeServicio::enviarSenial(pid_t pid, std::string proceso){
	int resultado = kill(pidGen, SIGINT);
	if (resultado == -1){
		std::string mensaje = std::string("Error en kill(): ") + std::string(strerror(errno));
		log.escribirEntrada("No pude enviar signal de finalizacion a " + proceso);
	}else
		log.escribirEntrada("Envie signal de finalizacion al " + proceso);
}

void EstacionDeServicio::finalizarAdministrador(){
	enviarSenial(pidAdmin, "administrador");
	wait(NULL); //admin
}

void EstacionDeServicio::liberarMediosDeComunicacion(){
	atencion.liberar();
	generacion.liberar();
	surtidores.eliminar();
}

void EstacionDeServicio::cerrar(){
	log.escribirEntrada("Comienzo CIERRE de estacion de servicio.");

	enviarSenial(pidGen, "generador");

	esperarCierre();	// esperar cierre

	liberarMediosDeComunicacion();

	log.escribirEntrada("Espere a todos mis hijos: empleados, jefe y generador de autos.");

	finalizarAdministrador();

	log.escribirEntrada("Ya se cerraron todos los empleados, el jefe, el generador y el administrador");
	log.mensajeCierre();
}

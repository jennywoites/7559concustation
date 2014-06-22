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

const std::string EstacionDeServicio::PIPE_ATENCION  = "atencion";
const std::string EstacionDeServicio::COLA_GENERACION  = "generacion";


EstacionDeServicio::EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos, int mediaAdmin): cantEmpleados (empleados),
	cantSurtidores (surtidores),
	mediaAutos (mediaGenAutos),
	mediaVerAdmin (mediaAdmin),
	pidGen (0),	pidAdmin (0), cantHijos(0){
	log.setTipo(Log::ENTRADA_PERSONAJE);
}


EstacionDeServicio::~EstacionDeServicio() {}

void EstacionDeServicio::agregarNombres(vector<std::string>& nombres){
	ifstream archNombres(ARCH_NOMBRES);	//abre archivo con nombres de empleados
    string linea;

    //por cada linea, se obtiene un nombre
    while (getline(archNombres, linea))
    	nombres.push_back(linea);

    archNombres.close(); //cierra archivo
}

std::string EstacionDeServicio::obtenerNombre(const vector<std::string>& nombres, int i) const{
	std::string nombre;
	int largo = nombres.size();
	if(largo == 0){
		stringstream ss;
		//obtengo nombre numerico, la lista esta vacia
		ss << i;
		nombre = ss.str();
	}else{
		//obtengo un nombre de la lista
		nombre = nombres.at(i%largo);
	}
	return nombre;
}

bool EstacionDeServicio::crearEmpleados(int tiempo){
	vector<std::string> nombres;
	agregarNombres(nombres); //obtiene los nombres desde el archivo
	pid_t pidEmpleado;

	for(int i = 0; i<cantEmpleados; i++){
		std::string nombre = obtenerNombre(nombres,i);

		//crea al empleado con su nombre, pipes y semaforo de surtidores
		try{
			Empleado e (nombre, atencion, surtidores, tiempo);
			log.escribirEntrada("Cree el empleado " + nombre);
			pidEmpleado = e.atenderAutos(cantSurtidores);
		}catch(const std::string &e){
			log.escribirEntrada("A razon del Log, no se pudo crear empleado: " + e);
			cerrar();
		}

		cantHijos++;
		if (pidEmpleado == 0) //como soy hijo, lo indico con true
			return true;
	}
	return false; //estacion crea todos los empleados, es padre
}

void EstacionDeServicio::crearAdmin(){
	try{
		Administrador a (mediaVerAdmin);
		cantHijos++;
		log.escribirEntrada("Creo mi administrador");
		pidAdmin = a.administrarCaja();
	}catch(const std::string &e){
		log.escribirEntrada("A razon del Log, no se pudo crear administrador: " + e);
	}
}

pid_t EstacionDeServicio::crearJefe(){
	pid_t pidJefe;
	try{
		Jefe j ("UltraAlterMaster", generacion, atencion);
		cantHijos++;
		log.escribirEntrada("Cree el jefe.");
		pidJefe = j.atenderAutos();
	}catch(const std::string &e){
		log.escribirEntrada("A razon del Log, no se pudo crear Jefe: " + e);
		cerrar();
	}

	return pidJefe;
}

void EstacionDeServicio::crearGenerador(){
	try{
		GeneradorAutos g (mediaAutos, generacion);
		cantHijos++;
		log.escribirEntrada("Creo el generador de autos");
		pidGen = g.generar();
	}catch(const std::string &e){
		log.escribirEntrada("A razon del Log, no se pudo crear Generador: " + e);
		cerrar();
	}
}

void EstacionDeServicio::cerrarPipe(PipeAutos& pipe, const std::string& tipo){
	try{
		pipe.cerrar();
		log.escribirEntrada("Me desadoso del pipe " + tipo);
	}catch(const std::string &e){
		log.escribirEntrada("No se pudo desadosar de pipe " + tipo + ": " + e);
	}
}

int EstacionDeServicio::abrir(int tiempoCaja){
	Log::setModo(Log::MODO_DEBUG);
	log.mensajeApertura();
	log.setEscritor("Estacion de Servicio");
	log.escribirEntrada("Comienzo a funcionar. ABIERTO");

	crearAdmin();
	if(pidAdmin == 0)
		return SOY_HIJO;

	try{
		surtidores.crear(ARCHIVO_ACCESO_SURTIDORES,ACCESO_SURTIDORES,cantSurtidores); //Semaforo de valor M = cantidad de surtidores
	}catch(const std::string &e){
		log.escribirEntrada("No se pudo crear el semaforo de surtidores: " + e);
		cerrar();
		return ERROR;
	}

	if(atencion.crear(ARCHIVO_ATENCION) )
		log.escribirEntrada("Creo el pipe de atencion de autos.");
	else{
		log.escribirEntrada("No se pudo crear el pipe de atencion");
		cerrar();
		return ERROR;
	}

	if(generacion.crear(ARCHIVO_GENERACION))
		log.escribirEntrada("Creo la cola de generacion de autos.");
	else{
		log.escribirEntrada("No se pudo crear la cola de generacion");
		cerrar();
		return ERROR;
	}

	bool soyHijo = crearEmpleados(tiempoCaja);
	if (soyHijo)
		return SOY_HIJO;

	log.escribirEntrada("Termine de crear todos los empleados.");

	pid_t pidJefe = crearJefe();
	if(pidJefe == 0)
		return SOY_HIJO;

	cerrarPipe(atencion, PIPE_ATENCION);

	crearGenerador();
	if(pidGen == 0)
		return SOY_HIJO;

	return OK;
}


void EstacionDeServicio::esperarCierre(){
	//Empleados + Jefe + Generador de Autos
	while (cantHijos > 1) { //no incluye Admin
		wait(NULL);
		cantHijos--;
	}
}

void EstacionDeServicio::enviarSenial(pid_t pid, std::string proceso){
	if (pid <=0)
		return ;
	int resultado = kill(pid, SIGINT);
	if (resultado == -1){
		std::string mensaje = std::string("Error en kill(): ") + std::string(strerror(errno));
		log.escribirEntrada("No pude enviar signal de finalizacion a " + proceso + " numero: ", pid);
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

	esperarCierre(); //Espero a que la estacion termine de atender los autos que quedaron pendientes

	try{
		liberarMediosDeComunicacion();
	}catch(const std::string &e){
		log.escribirEntrada("No se pudieron liberar todos los medios de comunicacion");
	}

	log.escribirEntrada("Espere a mis hijos: empleados, jefe y generador de autos.");

	finalizarAdministrador();

	log.escribirEntrada("Ya se cerraron todos los empleados, el jefe, el generador y el administrador");
	log.mensajeCierre();
}

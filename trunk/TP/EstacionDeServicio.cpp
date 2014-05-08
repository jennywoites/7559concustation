/*
 * EstacionDeServicio.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "EstacionDeServicio.h"

#include <sys/wait.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "Log.h"

#define ARCH_NOMBRES "TP/nombres.jem"

EstacionDeServicio::EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos){
	cantEmpleados = empleados;
	cantSurtidores = surtidores;
	mediaAutos = mediaGenAutos;
	generador = 0;
	administrador = 0;
}

EstacionDeServicio::~EstacionDeServicio() {}

void EstacionDeServicio::agregarNombres(vector<std::string>& nombres){
	//FIXME: Usar bien el archivo para obtener los nombres, verificar si existe

    ifstream archNombres(ARCH_NOMBRES);

    cout << "El archivo existe" << endl;

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

void EstacionDeServicio::crearEmpleados(const PipeAutos& pipe, const Semaforo& surtidores){
	vector<std::string> nombres;
	agregarNombres(nombres);
	for(int i = 0; i<cantEmpleados; i++){
		std::string nombre = obtenerNombre(nombres,i);
		Empleado e (nombre, pipe, surtidores);
		Log::enviarMensaje("Cree el empleado " + nombre);
		e.atenderAutos(cantSurtidores);
	}
}

void EstacionDeServicio::abrir(){
	Log log("log.jem");
	log.escribir();
	//Log::abrir_log();
	//Log::setEscritor("Estacion de Servicio");
	Log::enviarMensaje("Comienzo a funcionar. ABIERTO");

	Administrador a (3000000); //FIXME
	administrador = a.administrarCaja();
	Log::enviarMensaje("Creo mi administrador");

	//PipeAutos atencion;
	atencion.crear(ARCHIVO_ATENCION);
	Log::enviarMensaje("Creo el pipe de atencion de autos.");

	//Creo los empleados, el generador, el administrador y el jefe y los mando
	// a que atiendan. Cada uno crea sus procesos correspondientes
	surtidores.crear(ARCHIVO_ACCESO_SURTIDORES,ACCESO_SURTIDORES,cantSurtidores); //Semaforo de valor M= cantidad de surtidores
	crearEmpleados(atencion,surtidores);

	Log::enviarMensaje("Termine de crear todos los empleados.");

	//PipeAutos generacion;
	generacion.crear(ARCHIVO_GENERACION);
	Log::enviarMensaje("Creo el pipe de generacion de autos.");

	Jefe j ("UltraAlterMaster", generacion, atencion);
	Log::enviarMensaje("Cree el jefe.");
	j.atenderAutos();
	Log::enviarMensaje("Envie al jefe a atender autos.");

	//Cierro pipe de atencion
	atencion.cerrar();
	Log::enviarMensaje("Me desadoso del pipe de atencion.");

	GeneradorAutos g (mediaAutos, generacion);
	Log::enviarMensaje("Creo el generador de autos");
	generador = g.generar();
	Log::enviarMensaje("Envie al generador a generar autos.");

	//Cierro pipe de generacion
	generacion.cerrar();
	Log::enviarMensaje("Me desadoso del pipe de generacion de autos.");
}


void EstacionDeServicio::esperarCierre(){
	int i = cantEmpleados + 1 + 1; //Jefe + Generador de Autos
	while (i > 0) {
		wait(NULL);
		i--;
		Log::enviarMensaje("Ya termino uno de mis hijos. Aun me queda esperar a: ", i);
	}
}

void EstacionDeServicio::cerrar(){
	Log::enviarMensaje("Comienzo CIERRE de estacion de servicio.");

	kill(generador, SIGINT);	// envia senial a generador
	Log::enviarMensaje("Envie signal de finalizar al generador");

	esperarCierre();	// esperar cierre

	atencion.liberar();
	generacion.liberar();
	surtidores.eliminar();

	Log::enviarMensaje("Espere a todos mis hijos: empleados, jefe y generador de autos.");

	kill(administrador, SIGINT);	// envia senial a generador
	Log::enviarMensaje("Envie signal de finalizar al administrador");

	Log::cerrar_log();
}

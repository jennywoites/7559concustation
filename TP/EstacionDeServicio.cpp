/*
 * EstacionDeServicio.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "EstacionDeServicio.h"
#include "Log.h"
#include <sys/wait.h>

EstacionDeServicio::EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos){
	cantEmpleados = empleados;
	cantSurtidores = surtidores;
	mediaAutos = mediaGenAutos;
	generador = 0;
	administrador = 0;
}

EstacionDeServicio::~EstacionDeServicio() {}

void EstacionDeServicio::printDebug(std::string msj){
	//Log::enviarMensaje(msj);
}

void EstacionDeServicio::printDebug(std::string msj, int numero){
	//Log::enviarMensaje(msj, numero);
}


void EstacionDeServicio::crearEmpleados(const PipeAutos& pipe){
	for(int i = 0; i<cantEmpleados; i++){
		std::string nombre = "0" + i; //FIXME: Obtener el nombre para un empleado
		Empleado e (nombre, pipe);
		printDebug("Cree el empleado " + nombre);
		e.atenderAutos(cantSurtidores);
	}
}

void EstacionDeServicio::abrir(float plataInicial){
	Log log("log.jem");
	log.escribir();
	//Log::abrir_log();
	//Log::setEscritor("Estacion de Servicio");
	printDebug("Comienzo a funcionar. ABIERTO");

	Administrador a (30000000); //FIXME
	administrador = a.mirarDinero(plataInicial);
	printDebug("Creo mi administrador con dinero inicial $",plataInicial);

	PipeAutos atencion;
	printDebug("Creo el pipe de atencion de autos.");

	//Creo los empleados, el generador, el administrador y el jefe y los mando
	// a que atiendan. Cada uno crea sus procesos correspondientes
	crearEmpleados(atencion);

	printDebug("Termine de crear todos los empleados.");

	PipeAutos generacion;
	printDebug("Creo el pipe de generacion de autos.");

	Jefe j ("UltraAlterMaster", generacion, atencion);
	printDebug("Cree el jefe.");
	j.atenderAutos();
	printDebug("Envie al jefe a atender autos.");

	//Cierro pipe de atencion
	atencion.cerrar();
	printDebug("Me desadoso del pipe de atencion.");

	GeneradorAutos g (mediaAutos, generacion);
	printDebug("Creo el generador de autos");
	generador = g.generar();
	printDebug("Envie al generador a generar autos.");

	//Cierro pipe de generacion
	generacion.cerrar();
	printDebug("Me desadoso del pipe de generacion de autos.");
}


void EstacionDeServicio::esperarCierre(){
	int i = cantEmpleados + 1 + 1; //Jefe + Generador de Autos
	while (i > 0) {
		wait(NULL);
		i--;
		printDebug("Ya termino uno de mis hijos. Aun me queda esperar a: ", i);
	}
}

void EstacionDeServicio::cerrar(){
	printDebug("Comienzo CIERRE de estacion de servicio.");

	kill(generador, SIGINT);	// envia senial a generador
	printDebug("Envie signal de finalizar al generador");

	esperarCierre();	// esperar cierre
	printDebug("Espere a todos mis hijos: empleados, jefe y generador de autos.");

	kill(administrador, SIGINT);	// envia senial a generador
	printDebug("Envie signal de finalizar al administrador");

	//Log::cerrar_log();
}

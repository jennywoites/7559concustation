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

EstacionDeServicio::~EstacionDeServicio() {
	// TODO Auto-generated destructor stub
}

void EstacionDeServicio::crearEmpleados(const PipeAutos& pipe){
	for(int i = 0; i<cantEmpleados; i++){
		Empleado e ("0"+i, pipe);
		e.atenderAutos(cantSurtidores);
	}
}

void EstacionDeServicio::abrir(float plataInicial){
	//Log log("log.jem");
	//log.escribir();
	//Log::abrir_log();

	Administrador a (3000000); //FIXME
	administrador = a.mirarDinero(plataInicial);

	PipeAutos atencion;

	//Creo los empleados, el generador, el administrador y el jefe y los mando
	// a que atiendan. Cada uno crea sus procesos correspondientes
	crearEmpleados(atencion);

	PipeAutos generacion;

	Jefe j ("UltraAlterMaster", generacion, atencion);
	j.atenderAutos();

	//Cierro pipe de atencion
	atencion.cerrar();

	GeneradorAutos g (mediaAutos, generacion);
	generador = g.generar();

	//Cierro pipe de generacion
	generacion.cerrar();
}


void EstacionDeServicio::esperarCierre(){
	int i = cantEmpleados + 1 + 1;
	while (i >0) {
		wait(NULL);
		i--;
	}
}

void EstacionDeServicio::cerrar(){
	kill(generador, SIGINT);	// envia senial a generador
	esperarCierre();	// esperar cierre
	kill(administrador, SIGINT);	// envia senial a generador
	//Log::cerrar_log();
}

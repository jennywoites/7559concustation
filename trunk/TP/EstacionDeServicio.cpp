/*
 * EstacionDeServicio.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "EstacionDeServicio.h"
#include <sys/wait.h>

EstacionDeServicio::EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos){
	cantEmpleados = empleados;
	cantSurtidores = surtidores;
	mediaAutos = mediaGenAutos;
}

EstacionDeServicio::~EstacionDeServicio() {
	// TODO Auto-generated destructor stub
}

void EstacionDeServicio::crearEmpleados(const PipeAutos& pipe){
	for(int i = 0; i<cantEmpleados; i++){
		Empleado e ("0"+i, cantSurtidores, pipe);
		e.atenderAutos();
	}
}

void EstacionDeServicio::abrir(float plataInicial){
	//abrir Log
	//caja.abrir();
	PipeAutos atencion;
	crearEmpleados(atencion);

	PipeAutos generacion;

	Jefe j ("UltraAlterMaster", generacion, atencion);
	j.atenderAutos();
	GeneradorAutos g (mediaAutos, generacion);
	g.generar();
	Administrador a (100); //FIXME
	a.mirarDinero();
}


void EstacionDeServicio::esperarCierre(){
	while (wait(NULL)) {
	   if (errno == ECHILD) {
	      break;
	   }
	}
}

void EstacionDeServicio::cerrar(){
	// enviar senial a generador
	// esperar cierre
	// senial a admin
	caja.cerrar();
	// finalizar log
	esperarCierre();
}

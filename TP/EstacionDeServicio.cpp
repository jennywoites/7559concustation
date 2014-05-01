/*
 * EstacionDeServicio.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "EstacionDeServicio.h"
#include <sys/wait.h>

void EstacionDeServicio::crearEmpleados(){
	for(int i = 0; i<cantEmpleados; i++){
		Empleado e ("0"+i, cantSurtidores);
		e.atenderAutos();
	}
}

void EstacionDeServicio::abrir(float plataInicial){
	//abrir Log
	//caja.abrir();
	crearEmpleados();
	Jefe j ("UltraAlterMaster");
	j.atenderAutos();
	GeneradorAutos g (mediaAutos);
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

EstacionDeServicio::EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos){ {
	cantEmpleados = empleados;
	cantSurtidores = surtidores;
	mediaAutos = mediaGenAutos;
}

EstacionDeServicio::~EstacionDeServicio() {
}

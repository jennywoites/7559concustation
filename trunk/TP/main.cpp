/*
 * main.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Auto.h"
#include "Jefe.h"
#include "Empleado.h"
#include "EstacionDeServicio.h"
#include "ManejoTiempos.h"
#include "GeneradorAutos.h"
#include "Caja.h"
#include "Administrador.h"
#include "Log.h"
#include "MecanismoConcurrencia/PipeAutos.h"

void prueba1(void){
	Caja caja();
	Administrador admin(10000);
	admin.mirarDinero();
	wait(NULL);
	cout <<"Cierro la caja\n";
}

void pruebaPipeAutos(void){
	PipeAutos pipe;
	if (fork()==0){
		Auto a("ABC123", 10);
		pipe.escribirAuto(a);
	}else{
		Auto b;
		cout << pipe.leerAuto(&b) << endl;
		cout << b.getPatente() << endl;
	}
}

void pruebaGenPipeAutos(void){
	PipeAutos generacion;
	Jefe j ("UltraAlterMaster", generacion, generacion);
	j.atenderAutos();
	GeneradorAutos g (1000000, generacion);
	g.generar();
}

void pruebaAtPipeAutos(void){
	Administrador admin(3000000);
	pid_t adm = admin.mirarDinero();

	PipeAutos atencion;

    Empleado e ("0", atencion);
    e.atenderAutos(0);

    PipeAutos generacion;

	Jefe j ("UltraAlterMaster", generacion, atencion);
	j.atenderAutos();
	atencion.cerrar();

	GeneradorAutos g (1000000, generacion);

	pid_t gen = g.generar();
	generacion.cerrar();

	sleep(4);
	kill(gen, SIGINT);
	kill(adm, SIGINT);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
}

void prueba_log(){
	Log log("log.jem");
	log.escribir();
	Log::abrir_log();
	Log::enviarMensaje("El proceso principal abre el log");

	Caja caja;
	Administrador admin(10000);

	cout << "Voy a hacer que el admin empiece" <<endl;
	admin.mirarDinero();

	usleep(10000);
	Log::enviarMensaje("Voy a depositar 5");
	caja.abrir();
	caja.depositar(5);
	caja.cerrar();

	Log::cerrar_log();
	wait(NULL);//admin

	wait(NULL);//log
}

void prueba_memoria(){
	PipeAutos atencion;
	PipeAutos generacion;

	Administrador admin(300000);
	admin.mirarDinero();

	generacion.cerrar();
	atencion.cerrar();

	cout << "Prueba no finalizada" <<  endl;
	wait(NULL);
}

void prueba_signal_gen(){
	PipeAutos generacion;
	GeneradorAutos g (6000000, generacion);
	pid_t gen = g.generar();

	Administrador admin(300000);
	pid_t adm = admin.mirarDinero();

	sleep(7);
	kill(gen, SIGINT);
	kill(adm, SIGINT);
	generacion.cerrar();
	wait(NULL);
}

void crearEmpleados(const PipeAutos& pipe){
	for(int i = 0; i<1; i++){
		std::string nombre = "0" + i; //FIXME: Obtener el nombre para un empleado
		Empleado e (nombre, pipe);
		e.atenderAutos(0);
	}
}

void pruebaSurtido(){
	Administrador a (3000000000); //FIXME
	pid_t administrador = a.mirarDinero();

	PipeAutos atencion;

	//Creo los empleados, el generador, el administrador y el jefe y los mando
	// a que atiendan. Cada uno crea sus procesos correspondientes
	crearEmpleados(atencion);

	PipeAutos generacion;

	Jefe j ("UltraAlterMaster", generacion, atencion);
	j.atenderAutos();

	//Cierro pipe de atencion
	atencion.cerrar();

	GeneradorAutos g (300000, generacion);
	pid_t generador = g.generar();

	//Cierro pipe de generacion
	generacion.cerrar();

	sleep(2);
	kill(generador, SIGINT);
	kill(administrador, SIGINT);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
}

int main(void){
	//prueba1();
	//pruebaPipeAutos();
	//pruebaGenPipeAutos();
	//prueba_log();
	//pruebaAtPipeAutos();
	//prueba_memoria();
	//prueba_signal_gen();
	//pruebaSurtido();

	EstacionDeServicio e (2, 3, 300000);
	e.abrir();
	sleep(5);
	e.cerrar();

	exit(0);
}

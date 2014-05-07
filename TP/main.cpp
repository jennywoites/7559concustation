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
#include <sstream>

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
#include "MecanismoConcurrencia/Semaforo.h"

void prueba1(void){
	Caja caja();
	Administrador admin(10000);
	admin.administrarCaja();
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
	pid_t adm = admin.administrarCaja();

	PipeAutos atencion;
	Semaforo surtidor;
	surtidor.crear("main.cpp",'a',1);

    Empleado e ("0", atencion,surtidor);
    e.atenderAutos(0);

    PipeAutos generacion;

	Jefe j ("UltraAlterMaster", generacion, atencion);
	j.atenderAutos();
	atencion.cerrar();

	GeneradorAutos g (1000000, generacion);

	pid_t gen = g.generar();
	generacion.cerrar();
	surtidor.eliminar();

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
	admin.administrarCaja();

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
	admin.administrarCaja();

	generacion.cerrar();
	atencion.cerrar();

	cout << "Prueba no finalizada" <<  endl;
	wait(NULL);
}

void leer(const MemoriaCompartida<int>& sh){
	int lect = sh.leer();
	cout << lect << endl;
}

void pruebaMemComp(){
	if (fork() == 0){
		MemoriaCompartida<int> shmem("MecanismosConcurrencia/MemoriaCompartida.h", 'a');
		shmem.escribir(10);
		shmem.incrementar(-1);
		shmem.modificarValor(20);
		wait(NULL);
		shmem.liberar();
		exit(0);
	}else{
		MemoriaCompartida<int> shmem;
		MemoriaCompartida<int> shmem2;
		shmem.crear("MecanismosConcurrencia/MemoriaCompartida.h", 'a');
		shmem2 = shmem;
		usleep(2000);
		leer(shmem);
		shmem.liberar();
		exit(0);
	}
}

void prueba_signal_gen(){
	PipeAutos generacion;
	GeneradorAutos g (6000000, generacion);
	pid_t gen = g.generar();

	Administrador admin(300000);
	pid_t adm = admin.administrarCaja();

	sleep(7);
	kill(gen, SIGINT);
	kill(adm, SIGINT);
	generacion.cerrar();
	wait(NULL);
}

void crearEmpleados(const PipeAutos& pipe, const Semaforo& surtidores){
	for(int i = 0; i<1; i++){
		stringstream ss;
		ss << i;
		std::string nombre;
		nombre = ss.str();
		Empleado e (nombre, pipe, surtidores);
		e.atenderAutos(0);
	}
}

void pruebaSurtido(){
	Administrador a (3000000000); //FIXME
	pid_t administrador = a.administrarCaja();

	PipeAutos atencion;
	Semaforo surtidores;
	surtidores.crear("main.cpp",'A',1);

	//Creo los empleados, el generador, el administrador y el jefe y los mando
	// a que atiendan. Cada uno crea sus procesos correspondientes
	crearEmpleados(atencion,surtidores);

	PipeAutos generacion;

	Jefe j ("UltraAlterMaster", generacion, atencion);
	j.atenderAutos();

	//Cierro pipe de atencion
	atencion.cerrar();

	GeneradorAutos g (300000, generacion);
	pid_t generador = g.generar();

	//Cierro pipe de generacion
	generacion.cerrar();
	surtidores.eliminar();

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
	//pruebaMemComp();

	EstacionDeServicio e (10, 2, 3000000);
	e.abrir();
	sleep(5);
	e.cerrar();

	exit(0);
}

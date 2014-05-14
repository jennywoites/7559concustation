/*
 * Jefe.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "Jefe.h"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "constantesArchivos.h"

Jefe::Jefe(std::string n, const PipeAutos& gen, const PipeAutos& aten) {
	nombre = n;
	cantidadAtendida = 0;
	cantidadDespachada = 0;

	arribos = gen;
	envios = aten;
	log.setTipo(Log::ENTRADA_PERSONAJE);
}

Jefe::~Jefe(){}

bool Jefe::leerAuto(Auto* autito){
	log.escribirEntrada("Voy a leer un auto. Si no hay, me duermo.");
	bool status = arribos.leerAuto(autito);
	return status;
}

bool Jefe::atenderUnAuto(Auto& autito){
	log.escribirEntrada("Hay auto para ser atendido, patente " + string(autito.getPatente()));

	if (!hayEmpleados()){
		cantidadDespachada++;
		log.escribirEntrada("La cantidad de autos despachados es de: ", cantidadDespachada);
		mensajeDespachante(autito.getPatente());
		return true;
	}

	tomarEmpleado();
	if (! enviarAutoAEmpleado(autito) ){
		log.escribirEntrada("El empleado no pudo recibir el auto");
		return false;
	}

	cantidadAtendida++;
	log.escribirEntrada("La cantidad de autos que atendi es de: ", cantidadAtendida);
	return true;
}

bool Jefe::comenzarDia(){
	log.setEscritor("Jefe " + nombre);

	try{
		arribos.setearModo(Pipe::LECTURA);
		envios.setearModo(Pipe::ESCRITURA);
	}catch (std::string &e){
		cout << e << endl;
		log.escribirEntrada("No pude setear modos de comunicacion en Pipes");
		return false;
	}

	log.escribirEntrada("Se ha iniciado el Proceso.");

	try{
		cantEmpleadosDisponibles.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
		log.escribirEntrada("Se creo la memoria compartida: cantidad de empleados disponibles.");
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("No pude crear la memoria compartida: cantidad de empleados disponibles");
		return false;
	}

	try{
		SignalHandler::getInstance()->registrarHandler(SIGPIPE, &sigpipe_handler);
	}catch (std::string &e) {
		cout << e << endl;
		return false;
	}

	return true;
}

void Jefe::cerrarPipe(PipeAutos& pipe, const std::string& tipo){
	try{
		pipe.cerrar();
		log.escribirEntrada("Me desadoso del pipe " + tipo);
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("No se pudo desadosar de pipe " + tipo);
	}
}

void Jefe::cerrarCanales(){
	cerrarPipe(arribos, "generacion");
	cerrarPipe(envios, "atencion");
}

void Jefe::finalizarDia(){
	cerrarCanales();

	try{
		cantEmpleadosDisponibles.liberar();
		log.escribirEntrada("Libero la memoria compartida: cantidad de Empleados disponibles.");
	}catch(std::string &e){
		cout << e << endl;
		log.escribirEntrada("No fue posible liberar la memoria compartida: cantidad de empleados disponibles");
	}

	SignalHandler::destruir();
}

pid_t Jefe::atenderAutos(){
	pid_t id = fork();
	if (id != 0)
		return id;

	bool comienzo = comenzarDia();
	if(!comienzo){
		finalizarDia();
		log.escribirEntrada("Finalizo Proceso por ERROR.");
		return id;
	}

	Auto autito;

	while (leerAuto(&autito) and sigpipe_handler.getGracefulQuit() == 0){
		bool atendido = atenderUnAuto(autito);
		if (! atendido)
			log.escribirEntrada("No se pudo atender el auto " + autito.getPatente());
	}

	finalizarDia();
	log.escribirEntrada("Finalizo el proceso correctamente.");

	return id;
}

bool Jefe::hayEmpleados(){
	int cant_empleados = 0;
	try{
		cant_empleados = cantEmpleadosDisponibles.leer();
		log.escribirEntrada("La cantidad de empleados disponibles es ", cant_empleados);
	}catch(std::string &e){
		log.escribirEntrada("Error al leer de Memoria Compartidad Cantidad de Empleados. " + e);
		return false;
	}
	return (cant_empleados > 0);
}

void Jefe::tomarEmpleado(){
	try{
		log.escribirEntrada("Tomo un empleado disponible.");
		cantEmpleadosDisponibles.incrementar(-1);
	}catch(std::string &e){
		log.escribirEntrada("Error al toma empleado. " + e);
	}
}

void Jefe::mensajeDespachante(std::string patente){
	log.escribirEntrada("Se ha despachado al auto de patente " + patente + " por no haber empleados libres");
}

bool Jefe::enviarAutoAEmpleado(const Auto& autito){
	log.escribirEntrada("Envio el auto por el pipe hacia empleados");
	return envios.escribirAuto(autito);
}

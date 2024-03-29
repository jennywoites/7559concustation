/*
 * Jefe.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#include "Jefe.h"

#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "constantesArchivos.h"

Jefe::Jefe(std::string n, const ColaAutos& gen, const PipeAutos& aten):
		nombre(n),	cantidadAtendida(0),	cantidadDespachada(0),	arribos(gen),	envios(aten){
	log.setTipo(Log::ENTRADA_PERSONAJE);
}

Jefe::~Jefe(){}

bool Jefe::leerAuto(Auto& autito){
	log.escribirEntrada("Voy a leer un auto. Si no hay, me duermo.");
	bool status = arribos.leerAuto(autito);
	return status;
}

bool Jefe::atenderUnAuto(const Auto& autito){
	log.escribirEntrada("Hay auto para ser atendido, patente " + string(autito.getPatente()));

	//si no hay empleados, el auto se va
	if (!hayEmpleados()){
		cantidadDespachada++;
		log.escribirEntrada("La cantidad de autos despachados es de: ", cantidadDespachada);
		mensajeDespachante(autito);
		return true;
	}

	if (! tomarEmpleado() )
		return false;
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
		envios.setearModo(Pipe::ESCRITURA);
	}catch (const std::string &e){
		log.escribirEntrada("No pude setear modos de comunicacion en Pipes: " + e);
		return false;
	}

	log.escribirEntrada("Se ha iniciado el Proceso.");

	try{
		cantEmpleadosDisponibles.crear(ARCHIVO_CANTIDAD_EMPLEADOS, DISPONIBILIDAD_EMPLEADOS);
		log.escribirEntrada("Se creo la memoria compartida: cantidad de empleados disponibles.");
	}catch(const std::string &e){
		log.escribirEntrada("No pude crear la memoria compartida: cantidad de empleados disponibles: " + e);
		return false;
	}

	try{
		//la signal SIGPIPE se genera por escribir sobre el Pipe cuando los empleados tuvieron inconvenientes
		SignalHandler::getInstance()->registrarHandler(SIGPIPE, &sigpipe_handler);
		log.escribirEntrada("Registro el manejo de finalizacion SIGPIPE");
	}catch (const std::string &e) {
		log.escribirEntrada("No pude el manejo de finalizacion SIGPIPE: " + e);
		return false;
	}

	return true;
}

void Jefe::cerrarPipe(PipeAutos& pipe, const std::string& tipo){
	try{
		pipe.cerrar();
		log.escribirEntrada("Me desadoso del pipe " + tipo);
	}catch(const std::string &e){
		log.escribirEntrada("No se pudo desadosar de pipe " + tipo + ": " + e);
	}
}

void Jefe::cerrarCanales(){
	cerrarPipe(envios, "atencion");
}

void Jefe::finalizarDia(){
	cerrarCanales();

	try{
		cantEmpleadosDisponibles.liberar();
		log.escribirEntrada("Libero la memoria compartida: cantidad de Empleados disponibles.");
	}catch(const  std::string &e){
		log.escribirEntrada("No fue posible liberar la memoria compartida: cantidad de empleados disponibles: " + e);
	}

	SignalHandler::destruir();
}

pid_t Jefe::atenderAutos(){
	pid_t id = fork();
	if (id != 0)
		return id;
	//Hijo: Jefe

	bool comienzo = comenzarDia();
	//ha ocurrido un error que no permite continuar con la ejecucion
	if(!comienzo){
		finalizarDia();
		log.escribirEntrada("Finalizo Proceso por ERROR.");
		return id;
	}

	Auto autito;

	//mientras lleguen autos desde generacion, y mis empleados sigan atentos
	while (leerAuto(autito) and sigpipe_handler.getGracefulQuit() == 0){
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
	}catch(const std::string &e){
		log.escribirEntrada("Error al leer de Memoria Compartidad Cantidad de Empleados. " + e);
		return false;
	}
	return (cant_empleados > 0);
}

bool Jefe::tomarEmpleado(){
	try{
		log.escribirEntrada("Tomo un empleado disponible.");
		//jefe se encarga de indicar que el empleado ya no esta disponible
		cantEmpleadosDisponibles.incrementar(-1);
	}catch(const std::string &e){
		log.escribirEntrada("Error al tomar empleado. " + e);
		return false;
	}
	return true;
}

void Jefe::mensajeDespachante(const Auto& autito){
	std::string mensaje  = "Se ha despachado al auto de patente ";
	mensaje += autito.getPatente() + " y prioridad ";
	stringstream ss;
	ss << autito.getPrioridad();
	mensaje += ss.str();
	mensaje += " por no haber empleados libres";
	log.escribirEntrada(mensaje);
	cout << "\033[1;32mJefe: " << mensaje << "\033[0m" << endl;
}

bool Jefe::enviarAutoAEmpleado(const Auto& autito){
	log.escribirEntrada("Envio el auto por el pipe hacia empleados");
	return envios.escribirAuto(autito);
}

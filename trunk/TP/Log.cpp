/*
 * Log.cpp
 *
 *  Created on: May 10, 2014
 *      Author: ezequielgenender
 */

#include "Log.h"
#include <sstream>
#include <iostream>
#include <time.h>
#define LONG_TIEMPO 50
#define NO_TIPO -1

//std::string Log::archivo;
int Log::modo;

Log::Log() {
	lock.crear("log.jem");
	tipo = NO_TIPO;
}

Log::~Log() {
}

void Log::setEscritor(std::string writer){
	if (modo == MODO_NO_DEBUG)
		return;
	this->escritor = writer;
}

void Log::escribirEntrada(std::string mensaje){
	if (!permiteEntrada())
		return;

	std::string mensajeFinal = this->escritor + string(": ") + mensaje + string("\n");
	try{
		lock.tomarLock();
		lock.escribir(mensajeFinal.c_str(), mensajeFinal.size());
		lock.liberarLock();
	}catch(const std::string &e){
		cerr << e << endl;
		cerr << "No fue posible escribir la entrada en el log: " + mensaje + ". Causa: " + e << endl;
	}
}

void Log::escribirEntrada(std::string msj, int numero){
	stringstream ss;
	ss << numero;
	escribirEntrada(msj + ss.str());
}

void Log::escribirEntrada(std::string msj, float numero){
	stringstream ss;
	ss << numero;
	escribirEntrada(msj + ss.str());
}

void Log::setModo(int mode){
	Log::modo = mode;
}

void Log::mensajeInterno(std::string msj){
	msj += "\n";

	struct tm* tm_info;
	time_t timer;
	char buffer[LONG_TIEMPO];
	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer, LONG_TIEMPO, "%H:%M:%S", tm_info);

	std::string tiempo_msj = string(buffer) + string("\n\n\n");

	try{
		//tomo lock, para ser el unico log que escribe
		lock.tomarLock();
		lock.escribir(msj.c_str(), msj.size());
		lock.escribir(tiempo_msj.c_str(), tiempo_msj.size());
		lock.liberarLock();
		//libera lock, lo escrito queda unido
	}catch(const std::string &e){
		cerr << e << endl;
		cerr << "No fue posible escribir la entrada de tiempo en el log: " + e << endl;
	}
}

void Log::mensajeApertura(){
	mensajeInterno("********** SE INICIA NUEVA EJECUCION **********");
}

void Log::mensajeCierre(){
	mensajeInterno( "********** FIN DE EJECUCION **********");
}

void Log::setTipo(int tipo){
	this->tipo = tipo;
}

bool Log::permiteEntrada() const{
	if (tipo == NO_TIPO)
		return false;
	if (Log::modo == MODO_NO_DEBUG)
		return false;
	if (Log::modo == MODO_DEBUG_FULL)
		return true;

	//Modo no-completo
	switch (this->tipo){
		case ENTRADA_PERSONAJE:
			return true;
		case ENTRADA_MEMCOM:
			return false;
		case ENTRADA_PIPE:
			return false;
		case ENTRADA_SEMAFORO:
			return false;
		case ENTRADA_SIGNAL:
			return false;
		default:
			return false;
	}

}

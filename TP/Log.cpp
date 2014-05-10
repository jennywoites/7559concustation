/*
 * Log.cpp
 *
 *  Created on: May 10, 2014
 *      Author: ezequielgenender
 */

#include "Log.h"
#include <sstream>
#include <time.h>
#define LONG_TIEMPO 50

//std::string Log::archivo;
int Log::modo;

Log::Log() {
	lock.crear("log.jem");
}

Log::~Log() {
}

void Log::setEscritor(std::string writer){
	if (modo == MODO_NO_DEBUG)
		return;
	this->escritor = writer;
}

void Log::escribirEntrada(std::string mensaje){
	std::string mensajeFinal = this->escritor + string(": ") + mensaje + string("\n");
	lock.tomarLock();
	lock.escribir(mensajeFinal.c_str(), mensajeFinal.size());
	lock.liberarLock();
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
	modo = mode;
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

	lock.tomarLock();
	lock.escribir(msj.c_str(), msj.size());
	lock.escribir(tiempo_msj.c_str(), tiempo_msj.size());
	lock.liberarLock();
}

void Log::mensajeApertura(){
	mensajeInterno("********** SE INICIA NUEVA EJECUCION **********");
}

void Log::mensajeCierre(){
	mensajeInterno( "********** FIN DE EJECUCION **********");
}

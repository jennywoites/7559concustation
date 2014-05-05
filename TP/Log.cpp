/*
 * Log.cpp
 *
 *  Created on: 28/04/2014
 *      Author: martin
 */

#include "Log.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>

Pipe Log::pipe;
std::string Log::escritor;

using namespace std;

Log::Log(std::string path) {
	archivo = path;
	escritor = "";
	Log::pipe = Pipe();
	Log::pipe.crear(ARCHIVO_LOG);
}

Log::~Log() {

}

void Log::escribir(){

	if (fork() != 0)
		return;
	open(archivo.c_str(), O_CREAT, 0666);
	int salida = open(archivo.c_str(), O_WRONLY);
	//Cambio las lecturas y escrituras del proceso
	dup2(salida,1);
	Log::pipe.setearModo(Pipe::LECTURA);
//	dup2(Log::pipe.getFdLectura(),0);
	unsigned int numEntrada = 1;
	bool fin = false;

	while (!fin){
		mensaje_log_t msj;
		fin = pipe.leer(msj.mensaje, LONGITUD_MENSAJE * sizeof(char)) == 0;
		if (LOG_MODO == LOG_DEBUG and !fin)
			std::cout << numEntrada++ << ". " << msj.mensaje << endl;
	}
	std::cout <<"Se cerro el log"<<endl;
	exit(0);
}

Pipe* Log::abrir_log(){
	Log::pipe.setearModo(Pipe::ESCRITURA);
	return &Log::pipe;
}

void Log::cerrar_log(){
	pipe.cerrar();
}

void Log::setEscritor(std::string writer){
	Log::escritor = writer;
}

void Log::enviarMensaje(std::string msj, int numero){
	stringstream ss;
	ss << numero;
	enviarMensaje(msj + ss.str());
}

void Log::enviarMensaje(std::string msj, float numero){
	stringstream ss;
	ss << numero;
	enviarMensaje(msj + ss.str());
}


void Log::enviarMensaje(std::string msj){
	msj = escritor + string(": ") + msj;
	while (msj.size() < LONGITUD_MENSAJE){
		msj += '\0';
	}
	Log::pipe.escribir(msj.c_str(), msj.size());
}

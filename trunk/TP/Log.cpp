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

Pipe Log::pipe;

using namespace std;

Log::Log(std::string path) {
	archivo = path;
	Log::pipe = Pipe();
}

Log::~Log() {

}

void Log::escribir(){
	if (fork() != 0)
		return;
	open(archivo.c_str(), O_CREAT);
	int salida = open(archivo.c_str(), O_WRONLY);
	//Cambio las lecturas y escrituras del proceso
	dup2(salida,1);
	Log::pipe.setearModo(Pipe::LECTURA);
	dup2(Log::pipe.getFdLectura(),0);
	unsigned int numEntrada = 1;

	while (!std::cin.eof()){
		mensaje_log_t msj;
		std::cin.get(msj.mensaje, LONGITUD_MENSAJE * sizeof(char));
		if (LOG_MODO == LOG_DEBUG)
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

void Log::enviarMensaje(std::string msj){
	while (msj.size() < LONGITUD_MENSAJE-1){
		msj += '\0';
	}
	Log::pipe.escribir(msj.c_str(), msj.size());
}

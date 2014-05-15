#include "Pipe.h"
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>

Pipe :: Pipe() : lectura(false), escritura(false) {
}

void Pipe :: crear() {
	lectura = true;
	escritura = true;
	int creacion = pipe ( this->descriptores );
	if (creacion == -1){
		std::string mensaje = std::string("Error en pipe(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

Pipe::~Pipe() {
}

void Pipe::cerrarFd(int indice){
	if ( close ( this->descriptores[indice] ) == -1 ){
		std::string mensaje = std::string("Error en closePipe(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

void Pipe :: setearModo ( const int modo ) {
	if ( modo == LECTURA ) {
		cerrarFd(ESCRITURA);
		this->escritura = false;

	} else if ( modo == ESCRITURA ) {
		cerrarFd(LECTURA);
		this->lectura = false;
	}
}

ssize_t Pipe :: escribir ( const void* dato,int datoSize ) {
	if ( this->lectura == true ) {
		//si escribi, se cierra la lectura
		cerrarFd(LECTURA);
		this->lectura = false;
	}

	int escrito = write ( this->descriptores[ESCRITURA],dato,datoSize );
	if (escrito != -1)
		return escrito;

	std::string mensaje = std::string("Error en writePipe(): ") + std::string(strerror(errno));
	throw mensaje;
}

ssize_t Pipe :: leer ( void* buffer,const int buffSize ) {
	if ( this->escritura == true ) {
		//si lei, se cierra la escritura
		cerrarFd(ESCRITURA);
		this->escritura = false;
	}
	
	int leido = read ( this->descriptores[LECTURA],buffer,buffSize );
	if (leido != -1)
		return leido;

	std::string mensaje = std::string("Error en readPipe(): ") + std::string(strerror(errno));
	throw mensaje;
}

int Pipe :: getFdLectura () const {
	if ( this->lectura == true )
		return this->descriptores[LECTURA];
	else
		return -1;
}

int Pipe :: getFdEscritura () const {
	if ( this->escritura == true )
		return this->descriptores[ESCRITURA];
	else
		return -1;
}

void Pipe :: cerrar () {
	if ( this->lectura == true ) {
		cerrarFd(LECTURA);
		this->lectura = false;
	}

	if ( this->escritura == true ) {
		cerrarFd(ESCRITURA);
		this->escritura = false;
	}
}

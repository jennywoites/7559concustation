#include "Pipe.h"
#include <iostream>
#include <unistd.h>

Pipe :: Pipe() : lectura(false), escritura(false) {
}

void Pipe :: crear() {
	lectura = true;
	escritura = true;
	pipe ( this->descriptores );
	/*fcntl ( this->descriptors[0],F_SETFL,O_NONBLOCK );
	fcntl ( this->descriptors[1],F_SETFL,O_NONBLOCK );*/
}

Pipe::~Pipe() {
}

void Pipe :: setearModo ( const int modo ) {
	if ( modo == LECTURA ) {
		close ( this->descriptores[ESCRITURA] );
		this->escritura = false;

	} else if ( modo == ESCRITURA ) {
		close ( this->descriptores[LECTURA] );
		this->lectura = false;
	}
}

ssize_t Pipe :: escribir ( const void* dato,int datoSize ) {
	if ( this->lectura == true ) {
		close ( this->descriptores[LECTURA] );
		this->lectura = false;
	}

	return write ( this->descriptores[ESCRITURA],dato,datoSize );
}

ssize_t Pipe :: leer ( void* buffer,const int buffSize ) {
	if ( this->escritura == true ) {
		close ( this->descriptores[ESCRITURA] );
		this->escritura = false;
	}
	
	return  read ( this->descriptores[LECTURA],buffer,buffSize );
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
		close ( this->descriptores[LECTURA] );
		this->lectura = false;
	}

	if ( this->escritura == true ) {
		close ( this->descriptores[ESCRITURA] );
		this->escritura = false;
	}
}

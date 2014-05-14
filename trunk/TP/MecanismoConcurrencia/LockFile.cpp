#include "LockFile.h"

#include <iostream>
#include <errno.h>
#include <string.h>

LockFile::LockFile(){
	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
	fd = 0;
}

void LockFile :: crear ( const std::string nombre ) {
	this->nombre = nombre;
	int filedesc = open ( this->nombre.c_str(),O_CREAT|O_WRONLY,0777 );
	if(!filedesc){
		std::string mensaje = std::string("Error en open() en crear LockFile: ");
		throw mensaje;
		return;
	}
	this->fd = filedesc;
}

int LockFile :: tomarLock () {
	this->fl.l_type = F_WRLCK;
	int estado = fcntl ( this->fd,F_SETLKW,&(this->fl) );
	if(estado == -1){
		std::string mensaje = std::string("Error en fcntl() en tomarLock: ") + std::string(strerror(errno));
		throw mensaje;
	}
	return estado;
}

int LockFile :: liberarLock () {
	this->fl.l_type = F_UNLCK;
	int estado = fcntl( this->fd,F_SETLK,&(this->fl) );
	if(estado == -1){
		std::string mensaje = std::string("Error en fcntl() en liberarLock: ") + std::string(strerror(errno));
		throw mensaje;
	}
	return estado;
}

ssize_t LockFile :: escribir ( const void* buffer,const ssize_t buffsize ) const {
	int estado = lseek ( this->fd,0,SEEK_END );
	if(estado == -1){
		std::string mensaje = std::string("Error en lseek() en escribir ") + std::string(strerror(errno));
		throw mensaje;
	}
	return write ( this->fd,buffer,buffsize );
}

LockFile :: ~LockFile () {
	close ( this->fd );
}

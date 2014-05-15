#ifndef MemoriaCompartida_H_
#define MemoriaCompartida_H_

#include <sys/shm.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include "Semaforo.h"

using namespace std;

template <class T> class MemoriaCompartida {

private:
	int	shmId;
	T*	ptrDatos;

	int	cantidadProcesosAdosados() const;

	Semaforo control;

public:
	MemoriaCompartida();
	void crear(const std::string& archivo, const char letra);
	void liberar();

	MemoriaCompartida(const std::string& archivo,const char letra);
	MemoriaCompartida(const MemoriaCompartida& origen);
	~MemoriaCompartida();
	MemoriaCompartida<T>& operator= (const MemoriaCompartida& origen);
	void escribir(const T& dato);
	void incrementar(const T& valor);
	T leer() const;

	bool modificarValor(const T& valor);
};

template <class T> MemoriaCompartida<T>::MemoriaCompartida ():shmId(0),ptrDatos(NULL) {}

template <class T> void MemoriaCompartida<T>::crear ( const std::string& archivo,const char letra ) {
	try{
		control.crear(archivo.c_str(), letra, 1);
	}catch(std::string &e){
		std::string mensaje = std::string("Error en control.crear() de Memoria Compartida: ") + e;
		throw mensaje;
	}
	key_t clave = ftok ( archivo.c_str(),letra );

	if ( clave > 0 ) {
		this->shmId = shmget ( clave,sizeof(T),0644|IPC_CREAT );

		if ( this->shmId > 0 ) {
			void* tmpPtr = shmat ( this->shmId,NULL,0 );
			if ( tmpPtr != (void*) -1 ) {
				this->ptrDatos = static_cast<T*> (tmpPtr);
			} else {
				std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
				throw mensaje;
			}
		} else {
			std::string mensaje = std::string("Error en shmget(): ") + std::string(strerror(errno));
			throw mensaje;
		}
	} else {
		std::string mensaje = std::string("Error en ftok(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> void MemoriaCompartida<T>::liberar() {
	int errorDt = shmdt ( (void *) this->ptrDatos );

	if ( errorDt != -1 ) {
		int procAdosados = this->cantidadProcesosAdosados ();
		if ( procAdosados == 0 ) {
			shmctl ( this->shmId,IPC_RMID,NULL );
			try{
				control.eliminar();
			}catch(std::string &e){
				std::string mensaje = std::string("Error en control.eliminar(): ") + e;
				throw mensaje;
			}
		}
	} else {
		std::string mensaje = std::string("Error en shmdt(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

template <class T> MemoriaCompartida<T>::MemoriaCompartida ( const std::string& archivo,const char letra ):shmId(0),ptrDatos(NULL) {
	crear(archivo, letra);
}

template <class T> MemoriaCompartida<T>::MemoriaCompartida ( const MemoriaCompartida& origen ):
	shmId(origen.shmId),
	ptrDatos(origen.ptrDatos),
	control(origen.control){}

template <class T> MemoriaCompartida<T>::~MemoriaCompartida () {}

template <class T> MemoriaCompartida<T>& MemoriaCompartida<T>::operator= ( const MemoriaCompartida& origen ) {
	this->shmId = origen.shmId;
	this->ptrDatos = origen.ptrDatos;
	this->control = origen.control;

	return *this;
}

template <class T> void MemoriaCompartida<T>::escribir ( const T& dato ) {
	try{
		control.wait();
		*(this->ptrDatos) = dato;
		control.signal();
	}catch(std::string &e){
		std::string mensaje = std::string("Error en escribir(): ") + e;
		throw mensaje;
	}
}

template <class T> T MemoriaCompartida<T>::leer() const {
	T leido;
	try{
		control.wait();
		leido = *(this->ptrDatos);
		control.signal();
	}catch(std::string &e){
		std::string mensaje = std::string("Error en leer(): ") + e;
		throw mensaje;
	}
	return leido;
}

template<class T> void MemoriaCompartida<T>::incrementar(const T& valor){
	try{
		control.wait();
		*(this->ptrDatos) += valor;
		control.signal();
	}catch(std::string &e){
		std::string mensaje = std::string("Error en incrementar(): ") + e;
		throw mensaje;
	}
}

template <class T> int MemoriaCompartida<T> :: cantidadProcesosAdosados () const {
	shmid_ds estado;
	shmctl ( this->shmId,IPC_STAT,&estado );
	return estado.shm_nattch;
}

template <class T> bool MemoriaCompartida<T>::modificarValor(const T& valor){
	bool respuesta =false;
	try{
		control.wait();
		if (  *(this->ptrDatos) != valor ){
			respuesta = true;
			*(this->ptrDatos) = valor;
		}
		control.signal();
	}catch(std::string &e){
		std::string mensaje = std::string("Error en modificarValor(): ") + e;
		throw mensaje;
	}
	return respuesta;
}

#endif

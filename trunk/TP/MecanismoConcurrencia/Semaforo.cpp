#include "Semaforo.h"

#include <iostream>
#include <errno.h>
#include <string.h>
#include <sys/types.h>


Semaforo :: Semaforo ():id(0), valorInicial(0) {}

void Semaforo :: crear ( const std::string& nombre, char letra,const int valorInicial){
	creador.crear(nombre);
	creador.tomarLock();

	this->valorInicial = valorInicial; //valor que se dara al semaforo
	key_t clave = ftok (nombre.c_str(), letra);
	if(clave == -1){
		creador.liberarLock(); // lock para sincronizar unica inicializacion
		std::string mensaje = std::string("Error en ftok(): ") + std::string(strerror(errno));
		throw mensaje;
	}
	//supongo ser el primero que crea el semaforo
	this->id = semget ( clave, 1, 0666 | IPC_CREAT | IPC_EXCL );

	bool existia = (errno == EEXIST and this->id == -1);
	if(existia)	//ya existia, se obtiene sin crearlo
		this->id = semget(clave, 1, 0666);

	if(this->id == -1){
		creador.liberarLock();
		std::string mensaje = std::string("Error en semget(): ") + std::string(strerror(errno));
		throw mensaje;
		return;
	}

	if (! existia) // no existia, debe inicializarse
		this->inicializar ();
	creador.liberarLock(); //finaliza la exclusion mutua
}

Semaforo::~Semaforo() {}

int Semaforo :: inicializar () const {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = this->valorInicial; //asignar inicial
	int resultado = semctl ( this->id,0,SETVAL,init );
	if(resultado == -1){
		std::string mensaje = std::string("Error en semctl(): ") + std::string(strerror(errno));
		throw mensaje;
	}
	return resultado;
}

int Semaforo :: wait () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = -1;	// restar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	if(resultado == -1){
		std::string mensaje = std::string("Error en semop(): ") + std::string(strerror(errno));
		throw mensaje;
	}
	return resultado;
}

int Semaforo :: signal () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = 1;	// sumar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	if(resultado == -1){
		std::string mensaje = std::string("Error en semop(): ") + std::string(strerror(errno));
		throw mensaje;
	}
	return resultado;
}

int Semaforo :: getVal () const {
	int valor = semctl(this->id, 0, GETVAL, 0); //obtener valor
	if(valor == -1){
		std::string mensaje = std::string("Error en semctl(): ") + std::string(strerror(errno));
		throw mensaje;
	}
	return valor;
}

void Semaforo :: eliminar () const {
	int estado = semctl( this->id,0,IPC_RMID ); //remove
	if(estado == -1){
		std::string mensaje = std::string("Error en semctl(): ") + std::string(strerror(errno));
		throw mensaje;
	}
}

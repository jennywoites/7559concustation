#include "Semaforo.h"

Semaforo :: Semaforo ():id(0), valorInicial(0) {
}

void Semaforo :: crear ( const std::string& nombre, char letra,const int valorInicial){
	creador.crear(nombre);
	creador.tomarLock();
	this->valorInicial = valorInicial;
	key_t clave = ftok (nombre.c_str(), letra);
	this->id = semget ( clave,1,0666 | IPC_CREAT );

	this->inicializar ();
	creador.liberarLock();
}

Semaforo::~Semaforo() {
}

int Semaforo :: inicializar () const {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = this->valorInicial;
	int resultado = semctl ( this->id,0,SETVAL,init );
	return resultado;
}

int Semaforo :: wait () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = -1;	// restar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	return resultado;
}

int Semaforo :: signal () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = 1;	// sumar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	return resultado;
}

int Semaforo :: getVal () const {
	return semctl(this->id, 0, GETVAL, 0);
}

void Semaforo :: eliminar () const {
	semctl ( this->id,0,IPC_RMID );
}

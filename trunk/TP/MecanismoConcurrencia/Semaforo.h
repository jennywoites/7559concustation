#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include "LockFile.h"

class Semaforo {

private:
	int id;
	int valorInicial;
	LockFile creador;

	int inicializar () const;

public:
	Semaforo();
	~Semaforo();

	void crear(const std::string& nombre, char letra, const int valorInicial );
	int wait () const; // decrementa
	int signal () const; // incrementa
	int getVal () const; // obtiene el valor, DEBUG only
	void eliminar () const;
};

#endif /* SEMAFORO_H_ */

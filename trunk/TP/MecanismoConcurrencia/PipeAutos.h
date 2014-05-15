/*
 * PipeAutos.h
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#ifndef PIPEAUTOS_H_
#define PIPEAUTOS_H_

#include "Pipe.h"
#include "../Auto.h"
#include "Semaforo.h"

using namespace std;

class PipeAutos: public Pipe {
private:
	Semaforo controlLectura;
	static const char PIPE = 'p'; //caracter para creacion de estructuras
	static const int UN_SEMAFORO = 1; //cantidad de semaforos

public:
	PipeAutos();
	bool crear(const std::string& nombre);
	virtual ~PipeAutos();

	bool leerAuto ( Auto& autito );
	bool escribirAuto ( const Auto& autito );
	void liberar();
};

#endif /* PIPEAUTOS_H_ */

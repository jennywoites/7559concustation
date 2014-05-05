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
public:
	PipeAutos();
	void crear(const std::string& nombre);
	virtual ~PipeAutos();

	bool leerAuto ( Auto* autito );
	bool escribirAuto ( const Auto& autito );
	void cerrar();
	void liberar();
};

#endif /* PIPEAUTOS_H_ */

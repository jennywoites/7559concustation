/*
 * GeneradorAutos.h
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#ifndef GENERADORAUTOS_H_
#define GENERADORAUTOS_H_

#include "Auto.h"
#include "MecanismoConcurrencia/PipeAutos.h"

using namespace std;

class GeneradorAutos {
private:
	float media;
	PipeAutos envios;

public:
	GeneradorAutos(float media, const PipeAutos&);
	virtual ~GeneradorAutos();

	pid_t generar();

};

#endif /* GENERADORAUTOS_H_ */

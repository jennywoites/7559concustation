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
#include "MecanismoConcurrencia/SIGINT_Handler.h"
#include "MecanismoConcurrencia/SignalHandler.h"

class GeneradorAutos {
private:
	float media;
	int numAuto;
	PipeAutos envios;
	SIGINT_Handler sigint_handler;

public:
	GeneradorAutos(float media, const PipeAutos&);
	virtual ~GeneradorAutos();

	void comenzarDia();
	void finalizarDia();
	pid_t generar();

};

#endif /* GENERADORAUTOS_H_ */

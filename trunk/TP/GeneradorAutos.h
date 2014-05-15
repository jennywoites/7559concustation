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
#include "MecanismoConcurrencia/SIGPIPE_Handler.h"
#include "MecanismoConcurrencia/SignalHandler.h"
#include "Log.h"

class GeneradorAutos {
private:
	float media; //en tiempo, de generacion
	int numAuto;
	PipeAutos envios;
	SIGINT_Handler sigint_handler;
	SIGPIPE_Handler sigpipe_handler;
	Log log;

public:
	GeneradorAutos(float media, const PipeAutos&);
	virtual ~GeneradorAutos();

	pid_t generar();

private:
	bool comenzarDia();
	void finalizarDia();
	void cerrarPipe();
	void destruir();

};

#endif /* GENERADORAUTOS_H_ */

/*
 * GeneradorAutos.h
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#ifndef GENERADORAUTOS_H_
#define GENERADORAUTOS_H_

#include "Auto.h"
#include "MecanismoConcurrencia/ColaAutos.h"
#include "MecanismoConcurrencia/SIGINT_Handler.h"
#include "MecanismoConcurrencia/SIGPIPE_Handler.h"
#include "MecanismoConcurrencia/SignalHandler.h"
#include "Log.h"

class GeneradorAutos {
private:
	float media; //en tiempo, de generacion
	int numAuto;
	ColaAutos envios;
	SIGINT_Handler sigint_handler;
	SIGPIPE_Handler sigpipe_handler;
	Log log;

public:
	GeneradorAutos(float media, const ColaAutos&);
	virtual ~GeneradorAutos();

	pid_t generar();

private:
	bool comenzarDia();
	void finalizarDia();
	void cerrarPipe();
	void destruir();
	std::string mensajeCreacion(const Auto& autito);

};

#endif /* GENERADORAUTOS_H_ */

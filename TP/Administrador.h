/*
 * Administrador.h
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#ifndef ADMINISTRADOR_H_
#define ADMINISTRADOR_H_
#include "Caja.h"
#include "MecanismoConcurrencia/SIGINT_Handler.h"
#include "MecanismoConcurrencia/SignalHandler.h"

using namespace std;

class Administrador {
private:
	Caja caja;
	float plata_anterior;
	float media;
	SIGINT_Handler sigint_handler;

	void comenzarDia();
	void finalizarDia();
	void pensar();
	void mirarDinero();

public:
	Administrador(float);
	virtual ~Administrador();
	pid_t administrarCaja();
};

#endif /* ADMINISTRADOR_H_ */

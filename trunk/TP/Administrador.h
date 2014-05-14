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
#include "Log.h"

using namespace std;

class Administrador {
private:
	Caja caja;
	float plataVista;
	float media;
	SIGINT_Handler sigint_handler;
	Log log;

	bool comenzarDia();
	void finalizarDia();
	void pensar();
	void mirarDinero();
	void imprimirDinero();
	void destruir();

public:
	Administrador(float);
	virtual ~Administrador();
	pid_t administrarCaja();
};

#endif /* ADMINISTRADOR_H_ */

/*
 * Administrador.h
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#ifndef ADMINISTRADOR_H_
#define ADMINISTRADOR_H_
#include "Caja.h"

using namespace std;

class Administrador {
private:
	Caja caja;
	float plata_anterior;
	float media;

	void printDebug(std::string);
	void printDebug(std::string, int);

public:
	Administrador(float);
	virtual ~Administrador();
	pid_t mirarDinero();
};

#endif /* ADMINISTRADOR_H_ */

/*
 * GeneradorAutos.h
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#ifndef GENERADORAUTOS_H_
#define GENERADORAUTOS_H_

#include "Auto.h"
using namespace std;

class GeneradorAutos {
private:
	float media;

public:
	GeneradorAutos(float media);
	virtual ~GeneradorAutos();

	void generar();

};

#endif /* GENERADORAUTOS_H_ */

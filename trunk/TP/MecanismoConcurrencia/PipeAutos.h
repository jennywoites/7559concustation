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

using namespace std;

class PipeAutos: public Pipe {
public:
	PipeAutos();
	virtual ~PipeAutos();

	bool leerAuto ( Auto* autito );
	bool escribirAuto ( const Auto autito );
};

#endif /* PIPEAUTOS_H_ */

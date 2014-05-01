/*
 * PipeAutos.h
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#ifndef PIPEAUTOS_H_
#define PIPEAUTOS_H_

#include "Pipe.h"

using namespace std;

class PipeAutos: public Pipe {
public:
	PipeAutos();
	virtual ~PipeAutos();

	ssize_t leer ( void* buffer,const int buffSize );
};

#endif /* PIPEAUTOS_H_ */

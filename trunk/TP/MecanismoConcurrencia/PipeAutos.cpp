/*
 * PipeAutos.cpp
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#include "PipeAutos.h"

using namespace std;

PipeAutos::PipeAutos() {
	// TODO Auto-generated constructor stub

}

PipeAutos::~PipeAutos() {
	// TODO Auto-generated destructor stub
}

ssize_t PipeAutos :: leer ( void* buffer,const int buffSize ) {
	return Pipe::leer(buffer, buffSize);
}

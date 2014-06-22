/*
 * ColaAutos.h
 *
 *  Created on: 21/06/2014
 *      Author: jennifer
 */

#ifndef COLAAUTOS_H_
#define COLAAUTOS_H_

#include "Cola.h"
#include "../Auto.h"

using namespace std;

class ColaAutos {
private:
	static const char COLA = 'c'; //caracter para creacion de estructuras
	Cola<Auto::auto_t> envio;

public:
	ColaAutos();
	bool crear(const std::string& nombre);
	virtual ~ColaAutos();

	bool leerAuto ( Auto& autito );
	bool escribirAuto ( const Auto& autito );
	void liberar();
};

#endif /* COLAAUTOS_H_ */

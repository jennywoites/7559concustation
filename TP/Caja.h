/*
 * Caja.h
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#ifndef CAJA_H_
#define CAJA_H_
#include "MecanismoConcurrencia/MemoriaCompartida.h"

class Caja {
private:
	MemoriaCompartida<float> plata;
public:
	Caja();
	Caja(float plataInicial);
	virtual ~Caja();

	void depositar(float plata_a_agregar);

	float verMonto() const;

	void cerrar();

	void abrir();

};

#endif /* CAJA_H_ */

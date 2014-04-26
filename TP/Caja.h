/*
 * Caja.h
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#ifndef CAJA_H_
#define CAJA_H_

class Caja {
private:
	float plata;
public:
	Caja();
	Caja(int plataInicial);
	virtual ~Caja();

	void depositar(float plata_a_agregar);

	float verMonto() const;

};

#endif /* CAJA_H_ */

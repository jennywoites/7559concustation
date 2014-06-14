/*
 * Caja.h
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#ifndef CAJA_H_
#define CAJA_H_
#include "MecanismoConcurrencia/MemoriaCompartida.h"
#include "MecanismoConcurrencia/Semaforo.h"

class Caja {
private:
	MemoriaCompartida<float> plata;
	MemoriaCompartida<bool> hay_lector; // lo implementamos como bool porque solo hay un lector
	Semaforo escritores;  //nota: con este semaforo queda al pedo el semaforo de la memoria compartida

public:
	Caja();
	virtual ~Caja();

	void depositar(float plata_a_agregar);
	float verMonto() const;
	bool cerrar();
	bool abrir();
	void lectorQuiereUsar();
	void lectorDeja();
	void escritorQuiereUsar();
	void escritorDeja();
	void eliminar();
};

#endif /* CAJA_H_ */

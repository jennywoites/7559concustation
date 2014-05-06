/*
 * Empleado.h
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#ifndef EMPLEADO_H_
#define EMPLEADO_H_

#include <vector>
#include <stdexcept>

#include "MecanismoConcurrencia/MemoriaCompartida.h"
#include "MecanismoConcurrencia/PipeAutos.h"
#include "Caja.h"
#include "Auto.h"
#include "MecanismoConcurrencia/Semaforo.h"

using namespace std;

class Empleado {
private:
	static const float PRECIO_POR_LITRO = 10.07; //Precio YPF.... lloremos cada vez que aumentemos

	std::string nombre;
	int cantidadAtendidos;
	MemoriaCompartida<int> disponibilidad;
	Caja caja;
	PipeAutos arribos;
	Semaforo accesoSurtidores;
	std::vector< MemoriaCompartida<bool> > surtidores;

	static const bool USO = true;
	static const bool DESUSO = false;

	void printDebug(std::string);
	void printDebug(std::string, int);
	void printDebug(std::string, float);

public:
	Empleado(std::string, const PipeAutos&, const Semaforo&);
	virtual ~Empleado();

	void atenderAutos(int);
private:
	bool leerAuto(Auto*);
	int tomarSurtidor();
	void devolverSurtidor(int);
};

#endif /* EMPLEADO_H_ */

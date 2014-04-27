/*
 * Empleado.h
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#ifndef EMPLEADO_H_
#define EMPLEADO_H_

#include "MecanismoConcurrencia/MemoriaCompartida.h"
#include "Caja.h"
#include "Auto.h"
#include <vector>
using namespace std;

class Empleado {
private:
	static const float PRECIO_POR_LITRO = 10.07; //Precio YPF.... lloremos cada vez que aumentemos

	std::string nombre;
	int cantidadAtendidos;
	MemoriaCompartida<int> disponibilidad;
	Caja caja;
	std::vector<MemoriaCompartida<bool> > surtidores;
public:
	Empleado(std::string,int);
	virtual ~Empleado();

	void atenderAutos();
private:
	bool leerAuto(Auto*) const;
	int tomarSurtidor();
	void devolverSurtidor(int);
};

#endif /* EMPLEADO_H_ */

/*
 * Jefe.h
 *
 *  Created on: Apr 27, 2014
 *      Author: ezequielgenender
 */

#ifndef JEFE_H_
#define JEFE_H_
#include <string>
#include "Auto.h"
#include "MecanismoConcurrencia/MemoriaCompartida.h"
#include "MecanismoConcurrencia/PipeAutos.h"
using namespace std;

class Jefe {
private:
	MemoriaCompartida<int> cantEmpleadosDisponibles;
	std::string nombre;
	int cantidadDespachada;
	int cantidadAtendida;
	PipeAutos arribos;
	PipeAutos envios;

public:
	Jefe(std::string, const PipeAutos&, const PipeAutos&);
	virtual ~Jefe();

	void atenderAutos();
	bool leerAuto(Auto*);

private:

	bool hayEmpleados();
	void mensajeDespachante(std::string );
	void tomarEmpleado();
	bool enviarAutoAEmpleado(const Auto&);
	void printDebug(std::string);
	void printDebug(std::string,int);
};

#endif /* JEFE_H_ */

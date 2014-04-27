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

using namespace std;

class Jefe {
private:
	//pipe lectura desde el generador
	//memoria compartida de cant de empleados
	//pipe de escritura de autos hacia empleados
	MemoriaCompartida<int> cantEmpleadosDisponibles;
	std::string nombre;
	int cantidadDespachada;
	int cantidadAtendida;
public:
	Jefe(std::string);
	virtual ~Jefe();

	void atenderAutos();
	bool leerAuto(Auto*);

private:

	bool hayEmpleados();
	void mensajeDespachante(std::string );
	void tomarEmpleado();
	void enviarAutoAEmpleado(const Auto*);
};

#endif /* JEFE_H_ */

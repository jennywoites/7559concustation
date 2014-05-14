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
#include "Log.h"
using namespace std;

class Jefe {
private:
	MemoriaCompartida<int> cantEmpleadosDisponibles;
	std::string nombre;
	int cantidadDespachada;
	int cantidadAtendida;
	PipeAutos arribos;
	PipeAutos envios;
	Log log;

public:
	Jefe(std::string, const PipeAutos&, const PipeAutos&);
	virtual ~Jefe();

	pid_t atenderAutos();
	bool leerAuto(Auto*);

private:

	bool hayEmpleados();
	void atenderUnAuto(Auto&);
	void mensajeDespachante(std::string );
	void tomarEmpleado();
	bool comenzarDia();
	void finalizarDia();
	void cerrarCanales();
	bool enviarAutoAEmpleado(const Auto&);
	void printDebug(std::string);
	void printDebug(std::string,int);
	void cerrarPipe(PipeAutos& pipe, const std::string& tipo);
};

#endif /* JEFE_H_ */

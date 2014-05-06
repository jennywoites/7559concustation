/*
 * EstacionDeServicio.h
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#ifndef ESTACIONDESERVICIO_H_
#define ESTACIONDESERVICIO_H_

#include <vector>

#include "Caja.h"
#include "Empleado.h"
#include "Jefe.h"
#include "Administrador.h"
#include "GeneradorAutos.h"
#include "MecanismoConcurrencia/PipeAutos.h"
#include "constantesArchivos.h"
#include "MecanismoConcurrencia/Semaforo.h"


using namespace std;

class EstacionDeServicio {
private:
	int cantEmpleados;
	int cantSurtidores;
	int mediaAutos;
	pid_t generador;
	pid_t administrador;
	Caja caja;
	PipeAutos atencion;
	PipeAutos generacion;
	Semaforo surtidores;

private:

	void crearEmpleados(const PipeAutos&, const Semaforo&);
	void esperarCierre();
	void printDebug(std::string);
	void printDebug(std::string, int);
	void agregarNombres(vector<std::string>);
	std::string obtenerNombre(vector<std::string>, int);


public:
	EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos);
	virtual ~EstacionDeServicio();

	void abrir();
	void cerrar();
};

#endif /* ESTACIONDESERVICIO_H_ */

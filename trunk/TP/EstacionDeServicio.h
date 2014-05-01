/*
 * EstacionDeServicio.h
 *
 *  Created on: 01/05/2014
 *      Author: jennifer
 */

#ifndef ESTACIONDESERVICIO_H_
#define ESTACIONDESERVICIO_H_
#include "Caja.h"
#include "Empleado.h"
#include "Jefe.h"
#include "Administrador.h"
#include "GeneradorAutos.h"

using namespace std;

class EstacionDeServicio {
private:
	int cantEmpleados;
	int cantSurtidores;
	int mediaAutos;
	Caja caja;

	void crearEmpleados();
	void EstacionDeServicio::esperarCierre();
public:
	EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos);
	virtual ~EstacionDeServicio();

	void abrir(float plataInicial);
	void cerrar();
};

#endif /* ESTACIONDESERVICIO_H_ */

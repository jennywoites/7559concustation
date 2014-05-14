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
#include "MecanismoConcurrencia/Semaforo.h"
#include "Log.h"

using namespace std;

class EstacionDeServicio {
private:
	int cantEmpleados;
	int cantSurtidores;
	int mediaAutos;
	int mediaVerAdmin;
	pid_t pidGen;
	pid_t pidAdmin;
	Caja caja;
	PipeAutos atencion;
	PipeAutos generacion;
	Semaforo surtidores;
	Log log;

private:

	bool crearEmpleados(const PipeAutos&, const Semaforo&);
	void esperarCierre();
	void agregarNombres(vector<std::string>&);
	std::string obtenerNombre(vector<std::string>&, int);
	void crearAdmin();
	pid_t crearJefe();
	void crearGenerador();
	void enviarSenial(pid_t pid, std::string proceso);
	void finalizarAdministrador();
	void liberarMediosDeComunicacion();

public:

	static const int OK = 1;
	static const int SOY_HIJO = 0;
	static const int ERROR = -1;

	EstacionDeServicio(int empleados, int surtidores, int mediaGenAutos, int mediaAdmin);
	virtual ~EstacionDeServicio();

	int abrir();
	void cerrar();
};

#endif /* ESTACIONDESERVICIO_H_ */

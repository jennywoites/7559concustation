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
#include "MecanismoConcurrencia/Semaforo.h"
#include "Caja.h"
#include "Auto.h"
#include "Log.h"

using namespace std;

class Empleado {
private:
	static const float PRECIO_POR_LITRO = 10.07; //Precio YPF

	std::string nombre;
	int cantidadAtendidos;
	MemoriaCompartida<int> disponibilidad;
	Caja caja;
	PipeAutos arribos;
	PipeAutos generacion;
	Semaforo accesoSurtidores;
	std::vector< MemoriaCompartida<bool> > surtidores;
	Log log;

	static const bool USO = true;
	static const bool DESUSO = false;

public:
	Empleado(std::string, const PipeAutos&, const PipeAutos&, const Semaforo&);
	virtual ~Empleado();

	pid_t atenderAutos(int);
private:
	bool leerAuto(Auto&);
	void atenderUnAuto(Auto&);
	int tomarSurtidor();
	bool devolverSurtidor(int);
	bool comenzarDia();
	void finalizarDia();
	void cierreDeCaja();
	bool crearSurtidores(int);
	void cerrarPipe(PipeAutos& pipe, const std::string& tipo);
	void imprimirSurtidor(int, const Auto& autito) const;
	bool indicarDisponible(bool crear);
};

#endif /* EMPLEADO_H_ */

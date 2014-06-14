/*
 * Caja.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include "Caja.h"
#include "constantesArchivos.h"

using namespace std;

Caja::Caja() {}

Caja::~Caja() {}

void Caja::depositar(float plata_a_agregar){
	if (plata_a_agregar < 0 ){
		return;
	}
	try{
		plata.incrementar(plata_a_agregar);
	}catch(const std::string &e){
		cerr << "No fue posible depositar en caja. " << e << endl;
	}
}

float Caja::verMonto() const{
	float dinero = 0;
	try{
		dinero = plata.leer();
	}catch(const std::string &e){
		cerr << "No es posible ver monto de la caja. " << e << endl;
		return -1;
	}

	return dinero;
}

bool Caja::cerrar(){
	try{
		plata.liberar();
		hay_lector.liberar();

	}catch(const std::string &e){
		cerr << "No fue posible liberar la caja. " << e << endl;
		return false;
	}
	return true;
}

bool Caja::abrir(){
	try{
		plata.crear(ARCHIVO_CAJA, CAJA);
		hay_lector.crear(ARCHIVO_CAJA, CAJA+1); //se inicializa en 0?
		escritores.crear(ARCHIVO_CAJA, CAJA+2, 1);
	}catch(std::string &e){
		cerr << "No es posible abrir la caja. " << e << endl;
		return false;
	}
	return true;
}


void Caja::lectorQuiereUsar(){
	hay_lector.escribir(true);
	escritores.wait();
}

void Caja::lectorDeja(){
	hay_lector.escribir(false);
	escritores.signal();
}

void Caja::escritorQuiereUsar(){
	escritores.wait();
	//sleep(1); --> sleep para ver que la prioridad existe...
	//si cuando me dan a mi la caja veo que el administrador esta en la caja, y vuelvo a la cola
	while (hay_lector.leer()){
		cout << "Empleado: viene el administrador, asi que me vuelvo para la fila!" << endl;
		escritores.signal();
		escritores.wait();
	}
}

void Caja::escritorDeja(){
	escritores.signal();
}

void Caja::eliminar(){
	escritores.eliminar();
}

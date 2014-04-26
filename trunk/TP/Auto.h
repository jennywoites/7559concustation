/*
 * Auto.h
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#ifndef AUTO_H_
#define AUTO_H_
#include <string>

class Auto {
	private:
		std::string patente;
		int capacidadTanque;
		bool lleno;

		static const int cantidadLetras = 3;
		static const int cantidadNumeros = 3;
		static const int capacidadMin = 10;
		static const int capacidadMax = 70;

		//estas dos estan en milisegundos
		static const int tiempoPorLitroMin = 10;
		static const int tiempoPorLitroMax = 1000;

	public:

		/*Crea auto con patente aleatoria, y tanque aleatorio*/
		Auto();
		virtual ~Auto();

		/*Devuelve la patente del auto*/
		std::string getPatente() const;

		/*Llena el tanque del auto, informando cuanto se lleno. Esto demora un cierto tiempo.*/
		int llenar();
	private:
		int generarTanqueRandom() const;
		std::string generarPatenteRandom() const;
};

#endif /* AUTO_H_ */

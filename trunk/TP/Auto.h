/*
 * Auto.h
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#ifndef AUTO_H_
#define AUTO_H_
#include <string>

struct auto_serial{
	char patente[7];
	int capacidad;
	bool lleno;
};

class Auto {
	private:
		std::string patente;
		int capacidadTanque;
		bool lleno;

		static const unsigned int CANTIDAD_LETRAS = 3;
		static const unsigned int CANTIDAD_NUMEROS = 3;
		static const int CAPACIDAD_MIN = 10;
		static const int CAPACIDAD_MAX = 70;

		//estas dos estan en microsegundos
		static const int TIEMPO_POR_LITRO_MIN = 10;
		static const int TIEMPO_POR_LITRO_MAX = 1000;

	public:

		/*Crea auto con patente aleatoria, y tanque aleatorio*/
		Auto();
		Auto(std::string, int);
		virtual ~Auto();

		/*Devuelve la patente del auto*/
		std::string getPatente() const;

		/*Llena el tanque del auto, informando cuanto se lleno. Esto demora un cierto tiempo.*/
		int llenar();

		struct auto_serial serializar() const;
		void deserializar(struct auto_serial);
		void imprimir();

	private:
		int generarTanqueRandom() const;
		std::string generarPatenteRandom() const;
};

#endif /* AUTO_H_ */

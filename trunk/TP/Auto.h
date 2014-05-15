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

		static const unsigned int CANTIDAD_LETRAS = 3;
		static const unsigned int CANTIDAD_NUMEROS = 3;
		static const unsigned int CAR_TANQUE = 2; //El tanque tiene dos caracteres para definir la capacidad
		static const unsigned int CANTIDAD_BOOL = 1;
		static const unsigned int LARGO_SERIE = CANTIDAD_LETRAS + CANTIDAD_NUMEROS + CAR_TANQUE + CANTIDAD_BOOL + 2; //2: cantidad de separadores

		static const int CAPACIDAD_MIN = 10;
		static const int CAPACIDAD_MAX = 90;

		//estas dos estan en microsegundos
		static const int TIEMPO_POR_LITRO_MIN = 10;
		static const int TIEMPO_POR_LITRO_MAX = 1000;

	public:

		//se serializa con chars
		static const int LONG_SERIE = ((LARGO_SERIE + 1) * sizeof(char));

		/*Crea auto con patente aleatoria, y tanque aleatorio*/
		Auto();
		Auto(std::string, int);
		virtual ~Auto();

		/*Devuelve la patente del auto*/
		std::string getPatente() const;

		/*Llena el tanque del auto, informando cuanto se lleno. Esto demora un cierto tiempo.*/
		int llenar();

		std::string serializar() const;	//obtener en forma de bytes
		void deserializar(const std::string&);	//reconstruir a partir de bytes
		void imprimir() const;

	private:
		int generarTanqueRandom() const;
		std::string generarPatenteRandom() const;
};

#endif /* AUTO_H_ */

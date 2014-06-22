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
		long prioridad;

		static const unsigned int CANTIDAD_LETRAS = 3;
		static const unsigned int CANTIDAD_NUMEROS = 3;
		static const unsigned int CAR_TANQUE = 2; //El tanque tiene dos caracteres para definir la capacidad
		static const unsigned int CAR_PRIO = 1; //La prioridad tiene un caracter
		static const unsigned int CANTIDAD_BOOL = 1;
		static const unsigned int LARGO_PATENTE = CANTIDAD_LETRAS + CANTIDAD_NUMEROS + 1;
		static const unsigned int LARGO_SERIE = CANTIDAD_LETRAS + CANTIDAD_NUMEROS + CAR_TANQUE + CAR_PRIO + CANTIDAD_BOOL + 2; //2: cantidad de separadores

		static const int CAPACIDAD_MIN = 10;
		static const int CAPACIDAD_MAX = 90;

		//estas dos estan en microsegundos
		static const int TIEMPO_POR_LITRO_MIN = 10;
		static const int TIEMPO_POR_LITRO_MAX = 1000;

	public:

		static const int PRIOR_MIN = 1;
		static const int PRIOR_MAX = 2;

		struct auto_t{
			long mtype;
			char patente_auto[LARGO_PATENTE];
			int capacidad;
			bool estaLleno;
		};

		//se serializa con chars
		static const int LONG_SERIE = ((LARGO_SERIE + 1) * sizeof(char));

		/*Crea auto con patente aleatoria, y tanque aleatorio*/
		Auto();
		Auto(std::string, int, long);
		virtual ~Auto();

		/*Devuelve la patente del auto*/
		std::string getPatente() const;

		long getPrioridad() const;

		/*Llena el tanque del auto, informando cuanto se lleno. Esto demora un cierto tiempo.*/
		int llenar();

		std::string serializar() const;	//obtener en forma de bytes
		void deserializar(const std::string&);	//reconstruir a partir de bytes
		void imprimir() const;
		void reconstruir(struct auto_t);
		auto_t obtenerEstructura() const;

	private:
		int generarTanqueRandom() const;
		long generarPrioridadRandom() const;
		std::string generarPatenteRandom() const;
};

#endif /* AUTO_H_ */

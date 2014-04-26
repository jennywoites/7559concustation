/*
 * ManejoTiempos.h
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#ifndef MANEJOTIEMPOS_H_
#define MANEJOTIEMPOS_H_

/* MODULO PARA EL MANEJO DE TIEMPOS Y NUMEROS ALEATORIOS */

/* Funcion para inicializar la semilla para generación de números aleatorios*/
void inicializarRandom();

/* Funcion que devuelve un número (entero) al azar en el intervalo [min, max].
 * Precondicion: max > min.*/
int numeroAlAzar(int min, int max);


/* Devuelve la simulacion de una variable aleatoria exponencial, con una determinada media*/
float tiempoAlAzarExponencial(float media);


#endif /* MANEJOTIEMPOS_H_ */

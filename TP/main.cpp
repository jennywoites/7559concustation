/*
 * main.cpp
 *
 *  Created on: 26/04/2014
 *      Author: martin
 */

#include <stdio.h>
#include <iostream>
#include "Auto.h"
#include "Jefe.h"
#include "ManejoTiempos.h"
#include "GeneradorAutos.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "Caja.h"

int main(void){

	Caja caja(0);
	caja.depositar(1000);
	/*if (caja.verMonto() == 1000){
		cout << "Todo ok!"<<endl;
	}else{
		cout << "TODO MAL " << caja.verMonto() << endl;
	}*/

	exit(0);
}



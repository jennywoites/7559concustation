/*
 * Log.cpp
 *
 *  Created on: 28/04/2014
 *      Author: martin
 */

#include "Log.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

Log::Log(std::string path) {
	archivo = path;
}

Log::~Log() {

}

void Log::escribir(){
	if (fork() != 0)
		return;
	open(archivo.c_str(), O_CREAT);
	int salida = open(archivo.c_str(), O_WRONLY);
	dup2(salida,1);

	while (!std::cin.eof()){
		char letra;
		std::cin >> letra;
		if (LOG_MODO == LOG_DEBUG)
			std::cout << letra;
	}
	std::cout <<"Se cerro el log"<<endl;
	exit(0);
}

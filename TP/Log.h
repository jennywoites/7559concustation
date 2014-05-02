/*
 * Log.h
 *
 *  Created on: 28/04/2014
 *      Author: martin
 */

#ifndef LOG_H_
#define LOG_H_
#include <string>
#include "MecanismoConcurrencia/Pipe.h"

using namespace std;
#define LONGITUD_MENSAJE 140

typedef struct mensaje_log{
	char mensaje[LONGITUD_MENSAJE];
}mensaje_log_t;


/* USO: en el proceso principal hay que crear y mandar a escribir al log.
 * En los procesos que quieran escribir al log solamente deben hacer "abrir_log",
 * cada vez que quieran mandar algo deben llamar a "enviarMensaje", y al terminar
 * deben llamar a cerrar().
 * */

class Log {
private:
	std::string archivo;

	static Pipe pipe;
	static int valor;

	static const int LOG_DEBUG = 0;
	static const int LOG_RELEASE = 1;
	static const int LOG_MODO = LOG_DEBUG;

public:
	Log(std::string path);
	virtual ~Log();

	void escribir();


	static Pipe* abrir_log();
	static void cerrar_log();
	static void enviarMensaje(std::string);
};

#endif /* LOG_H_ */

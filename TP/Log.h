#ifndef LOG_H_
#define LOG_H_

#include "MecanismoConcurrencia/LockFile.h"

using namespace std;

class Log {
private:

	LockFile lock;
	std::string escritor;

	//static const std::string archivo = string("log.jem");

	static int modo;
public:
	static const int MODO_DEBUG = 0;
	static const int MODO_DEBUG_FULL = 1;
	static const int MODO_NO_DEBUG = 2;

	Log();

	void setEscritor(std::string);
	void escribirEntrada(std::string);
	void escribirEntrada(std::string, int);
	void escribirEntrada(std::string, float);
	void mensajeApertura();
	void mensajeCierre();

	static void setModo(int);
	virtual ~Log();

private:
	void mensajeInterno(std::string);
};

#endif /* LOG_H_ */

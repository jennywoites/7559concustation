#ifndef LOG_H_
#define LOG_H_

#include "MecanismoConcurrencia/LockFile.h"

using namespace std;

class Log {
private:

	LockFile lock;
	std::string escritor;
	int tipo;

	static int modo;
public:
	static const int MODO_DEBUG = 0;
	static const int MODO_DEBUG_FULL = 1;
	static const int MODO_NO_DEBUG = 2;

	static const int ENTRADA_PERSONAJE = 0;
	static const int ENTRADA_SEMAFORO = 1;
	static const int ENTRADA_MEMCOM = 2;
	static const int ENTRADA_PIPE = 3;
	static const int ENTRADA_SIGNAL = 4;

	Log();

	void setEscritor(std::string);
	void escribirEntrada(std::string);
	void escribirEntrada(std::string, int);
	void escribirEntrada(std::string, float);
	void mensajeApertura();
	void mensajeCierre();
	void setTipo(int);

	static void setModo(int);
	virtual ~Log();

private:
	void mensajeInterno(std::string);
	bool permiteEntrada();
};

#endif /* LOG_H_ */

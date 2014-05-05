#ifndef PIPE_H_
#define PIPE_H_

#include <unistd.h>
#include <fcntl.h>
#include "Semaforo.h"

class Pipe {

private:
	int descriptores[2];
	bool lectura;
	bool escritura;
	Semaforo controlLectura;

public:
	static const int LECTURA = 0;
	static const int ESCRITURA = 1;

	Pipe();
	void crear(const std::string& nombre);
	virtual ~Pipe();

	void setearModo ( const int modo );

	ssize_t escribir ( const void* dato,const int datoSize );
	ssize_t leer ( void* buffer,const int buffSize );

	int getFdLectura () const;
	int getFdEscritura () const;

	void cerrar ();
};

#endif /* PIPE_H_ */
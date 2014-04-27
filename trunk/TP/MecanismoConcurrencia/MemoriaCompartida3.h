/*
 * MemoriaCompartida.h
 *
 *  Created on: 18/04/2014
 *      Author: jennifer
 */

#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_
#include <iostream>

using namespace std;

template <class Tipo> class MemoriaCompartida {
private:
	int shmId;
	Tipo* datos;

	static const int RDWR = 0;

	std::string lanzarError(const std::string& mensaje);
	int cantAdosados() const;
	void adosar();

public:
	MemoriaCompartida();
	MemoriaCompartida(const std::string& archivo,const char projId);
	MemoriaCompartida(const MemoriaCompartida& origen);
	~MemoriaCompartida();

	MemoriaCompartida<Tipo>& operator=(const MemoriaCompartida & origen);

	Tipo leer() const;
	void escribir(Tipo escritura);
	void incrementar(Tipo valor);
	void decrementar(Tipo valor);
};

#endif /* MEMORIACOMPARTIDA_H_ */

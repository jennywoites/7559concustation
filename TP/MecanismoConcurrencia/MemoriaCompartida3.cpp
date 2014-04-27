/*
 * MemoriaCompartida.cpp
 *
 *  Created on: 18/04/2014
 *      Author: jennifer
 */

#include "MemoriaCompartida.h"
#include "sys/ipc.h"
#include "sys/shm.h"
#include <string.h>
#include <errno.h>

//template<class Tipo> std::string MemoriaCompartida<Tipo>::lanzarError(const std::string& mensaje){
//	return std::string(mensaje) + ": " + std::string(strerror(errno));
//}
//
//template<class Tipo> int MemoriaCompartida<Tipo>::cantAdosados() const{
//	shmid_ds estado;
//	shmctl(shmId, IPC_STAT, &estado);
//	return estado.shm_nattch;
//}
//
//
//template<class Tipo> MemoriaCompartida<Tipo>::MemoriaCompartida(): shmId (0), datos(NULL){
//}
//
//template<class Tipo> void MemoriaCompartida<Tipo>::adosar(){
//	if(shmId <= 0)
//		return;
//	void* temporal = shmat(shmId, NULL, RDWR);
//	if (temporal == (void*)-1){
//		throw lanzarError("Error en shmat()");
//	}
//	datos = static_cast<Tipo*>(temporal);
//}
//
//template<class Tipo> MemoriaCompartida<Tipo>::MemoriaCompartida(const std::string& archivo, const char projId){
//	int clave = ftok(archivo.c_str(), projId);
//	if (clave == -1){
//		throw lanzarError("Error en ftok()");
//	}
//	shmId = shmget(clave, sizeof(Tipo), IPC_CREAT|0644);
//	if (shmId == -1){
//		throw lanzarError("Error en shmget()");
//	}
//	adosar();
//}
//
//template<class Tipo> MemoriaCompartida<Tipo>::MemoriaCompartida(const MemoriaCompartida& origen): shmId(origen.shmId), datos(origen.datos){
//}
//
//template<class Tipo> MemoriaCompartida<Tipo>& MemoriaCompartida<Tipo>::operator=(const MemoriaCompartida& origen){
//	return *(new MemoriaCompartida(origen));
//}
//
//template<class Tipo> Tipo MemoriaCompartida<Tipo>::leer() const{
//	return *datos;
//}
//
//template<class Tipo> void MemoriaCompartida<Tipo>::escribir(Tipo escritura){
//	if (!datos)
//		return;
//	*datos = escritura;
//}
//
//template<class Tipo> void MemoriaCompartida<Tipo>::incrementar(Tipo valor){
//	*datos += valor;
//}
//
//template<class Tipo> void MemoriaCompartida<Tipo>::decrementar(Tipo valor){
//	*datos -= valor;
//}
//
//template<class Tipo> MemoriaCompartida<Tipo>::~MemoriaCompartida(){
//	int borrado = shmdt(static_cast<void*>(datos));
//	if (borrado == -1){
//		throw lanzarError("Error en shmdt()");
//	}
//	if (cantAdosados() == 0){
//		borrado = shmctl(shmId, IPC_RMID, NULL);
//		if (borrado == -1){
//			throw lanzarError("Error en shmctl(REMOVE)");
//		}
//	}
//}
//

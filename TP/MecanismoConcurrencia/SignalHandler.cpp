#include "SignalHandler.h"
#include <memory.h>
#include <iostream>
#include <string.h>
#include <errno.h>

SignalHandler* SignalHandler :: instance = NULL;
EventHandler* SignalHandler :: signal_handlers [ NSIG ];

SignalHandler :: SignalHandler () {
}

SignalHandler* SignalHandler :: getInstance () {

	if ( instance == NULL )
		instance = new SignalHandler ();

	return instance;
}

void SignalHandler :: destruir () {
	if ( instance != NULL ) {
		delete ( instance );
		instance = NULL;
	}
}

EventHandler* SignalHandler :: registrarHandler ( int signum,EventHandler* eh ) {

	EventHandler* old_eh = SignalHandler :: signal_handlers [ signum ];
	SignalHandler :: signal_handlers [ signum ] = eh;

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SignalHandler :: dispatcher;
	sigemptyset ( &sa.sa_mask );	// inicializa la mascara de seniales a bloquear durante la ejecucion del handler como vacio
	sigaddset ( &sa.sa_mask,signum );
	sa.sa_flags = 0;
	int resultado = sigaction ( signum,&sa,0 );	// cambiar accion de la senial
	if (resultado == -1){
		std::string mensaje = std::string("Error en sigaction(): ") + std::string(strerror(errno));
		SignalHandler :: signal_handlers [ signum ] = old_eh;
		throw mensaje;
	}

	return old_eh;
}

void SignalHandler :: dispatcher ( int signum ) {

	if ( SignalHandler :: signal_handlers [ signum ] != 0 ){
		try{
			SignalHandler :: signal_handlers [ signum ]->handleSignal ( signum );
		}catch ( const std::exception & e ) {
			std::cerr << e.what() << std::endl;
		}
	}
}

int SignalHandler :: removerHandler ( int signum ) {

	SignalHandler :: signal_handlers [ signum ] = NULL;
	return 0;
}

#ifndef SIGPIPE_HANDLER_H_
#define SIGPIPE_HANDLER_H_

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"

class SIGPIPE_Handler : public EventHandler {

	private:
		sig_atomic_t gracefulQuit;

	public:

		SIGPIPE_Handler () : gracefulQuit(0) {
		}

		~SIGPIPE_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGPIPE );
			this->gracefulQuit = 1;
			return 0;
		}

		sig_atomic_t getGracefulQuit () const {
			return this->gracefulQuit;
		}

};

#endif /* SIGPIPE_HANDLER_H_ */

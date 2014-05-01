/*
 * Log.h
 *
 *  Created on: 28/04/2014
 *      Author: martin
 */

#ifndef LOG_H_
#define LOG_H_
#include <string>

using namespace std;

class Log {
private:
	std::string archivo;

	static const int LOG_DEBUG = 0;
	static const int LOG_RELEASE = 1;
	static const int LOG_MODO = LOG_DEBUG;

public:
	Log(std::string path);
	virtual ~Log();

	void escribir();
};

#endif /* LOG_H_ */

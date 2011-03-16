/*
 * Logger.h
 *
 *  Created on: Mar 16, 2011
 *      Author: xmicsta
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

namespace osock
{

/* Sole purpose of this class is to discard all that goes into it */
struct NullStream: std::ostream
{
	struct nullbuf: std::streambuf
	{
		int overflow(int c)
		{
			return traits_type::not_eof(c);
		}
	} m_sbuf;
	NullStream() :
		std::ios(&m_sbuf), std::ostream(&m_sbuf)
	{
	}
};

class Logger
{
private:
	Logger();
public:
	typedef enum { logOff, logDebug, logWarn, logError } logLevel;

	static NullStream nullingstream;
	static logLevel printLoglevel;
	static std::ostream& Print(logLevel loglevel);
	virtual ~Logger();
};

}	//namespace osock

#endif /* LOGGER_H_ */

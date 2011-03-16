/*
 * Logger.cpp
 *
 *  Created on: Mar 16, 2011
 *      Author: xmicsta
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <Utilities/Logger.h>

namespace osock {

Logger::logLevel Logger::printLoglevel = Logger::logOff;
NullStream Logger::nullingstream;

Logger::Logger()
{
}

Logger::~Logger()
{
}

std::ostream& Logger::Print(logLevel loglevel)
{
	if(loglevel < printLoglevel) {
		return nullingstream;
	} else {
		if (loglevel <= logDebug)
			return std::cout;
		else
			return std::cerr;
	}
}

}

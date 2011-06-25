/*
	Copyright 2011, Michal Stawinski <michal.stawinski@gmail.com>

	This file is part of libsockets- C++ wrapper over OpenSSL and raw sockets.

	libsockets is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libsockets is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with libsockets.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <Utilities/Logger.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>

#include "osockconfig.h"

namespace osock
{

Logger::Logger(std::string logname, int loglevel) :
		itsLogname(logname)
{
	if (loglevel == -1)
		loglevel = LOGLEVEL;
//		loglevel = logDebug;

	getLoggers().insert(loggerDesc_t(logname, static_cast<logLevel>(loglevel)));
}

Logger::~Logger()
{
}

std::map<std::string, Logger::logLevel>& Logger::getLoggers()
{
	static std::map<std::string, logLevel>* loggersDescs = new std::map<std::string, logLevel>();
	return *loggersDescs;
}

NullStream& Logger::getNullStream()
{
	static NullStream* nstream = new NullStream();
	return *nstream;
}

std::ostream& Logger::out(logLevel loglevel)
{
	std::map<std::string, logLevel>::const_iterator it;

	//XXX consecutive accesses to map can result in data corruption if we were rescheduled
	it = getLoggers().find(itsLogname);
	assert(it != getLoggers().end());

//	std::cout << to_string(loglevel) << "/" << to_string((*it).second) << " " << itsLogname << std::endl;

	if(loglevel < (*it).second) {
		return getNullStream();
	} else {
		if (loglevel <= logInfo)
			return std::cout << getpid() << " ";
		else
			return std::cerr << getpid() << " ";
	}
}
}

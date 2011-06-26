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

	logConfig config;
	config.CurrentLevel = static_cast<logLevel>(loglevel);
	config.DefaultLevel = static_cast<logLevel>(loglevel);

	getLoggers().insert(loggerDesc_t(logname, config));
}

Logger::~Logger()
{
}

std::map<std::string, Logger::logConfig>& Logger::getLoggers()
{
	static std::map<std::string, Logger::logConfig>* loggersDescs = new std::map<std::string, Logger::logConfig>();
	return *loggersDescs;
}

NullStream& Logger::getNullStream()
{
	static NullStream* nstream = new NullStream();
	return *nstream;
}

void Logger::ForceLoglevel(logLevel level)
{
	std::map<std::string, logConfig>::iterator it;
	it = getLoggers().begin();

	for (; it != getLoggers().end(); ++it) {
		(*it).second.CurrentLevel = level;
	}
}

void Logger::ForceLoglevel(logLevel level, std::string module)
{
	std::map<std::string, logConfig>::iterator it;
	it = getLoggers().find(module);
	assert(it != getLoggers().end());
	(*it).second.CurrentLevel = level;
}

void Logger::RestoreLoglevel()
{
	std::map<std::string, logConfig>::iterator it;
	it = getLoggers().begin();

	for (; it != getLoggers().end(); ++it) {
		(*it).second.CurrentLevel = (*it).second.DefaultLevel;
	}
}

void Logger::RestoreLoglevel(std::string module)
{
	std::map<std::string, logConfig>::iterator it;
	it = getLoggers().find(module);
	assert(it != getLoggers().end());
	(*it).second.CurrentLevel = (*it).second.DefaultLevel;

}

std::ostream& Logger::out(logLevel loglevel)
{
	std::map<std::string, logConfig>::const_iterator it;

	//XXX consecutive accesses to map can result in data corruption if we were rescheduled
	it = getLoggers().find(itsLogname);
	assert(it != getLoggers().end());

//	std::cout << to_string(loglevel) << "/" << to_string((*it).second.CurrentLevel) << " " << itsLogname << std::endl;

	if(loglevel < (*it).second.CurrentLevel) {
		return getNullStream();
	} else {
		if (loglevel <= logInfo)
			return std::cout << getpid() << " ";
		else
			return std::cerr << getpid() << " ";
	}
}
}

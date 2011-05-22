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

#ifndef DEFINES_H_
#define DEFINES_H_

//#include <Utilities/Logger.h>

#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <sstream>


namespace osock
{

#define LOGLEVEL_DBG	4
#define LOGLEVEL_NFO	3
#define LOGLEVEL_WRN	2
#define LOGLEVEL_ERR	1

namespace http
{
#define 	NEWLINE 	"\r\n"
}

#define UNUSED(x) (void)x

template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}
} //namespace osock

#endif /* DEFINES_H_ */

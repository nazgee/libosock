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
#include <Exception/Exception.h>

#include <string.h>
#include <stdio.h>

namespace osock
{
Exception::Exception(const std::string& msg) :
	itsMessage(msg)
{
	DBG_CONSTRUCTOR;
	DBG << itsMessage << std::endl;
}
Exception::~Exception(void) throw ()
{
	DBG_DESTRUCTOR;
}

const char* Exception::what() const throw ()
{
	return itsMessage.c_str();
}

HttpException::HttpException(const std::string& msg, const http::Status& status) :
	Exception("HttpException: " + msg), itsStatus(status)
{
}

HttpException::~HttpException(void) throw ()
{
}

http::Status HttpException::getStatus() const
{
	return itsStatus;
}
}

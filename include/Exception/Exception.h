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

#ifndef OSOCEXCEPTIONS_H_
#define OSOCEXCEPTIONS_H_

#include <openssl/err.h>
#include <string.h>	//strerror()
#include <errno.h>
#include <iostream>
#include <stdexcept>

#include "../Message/http/Status.h"

namespace osock
{
class Exception : public std::exception
{
private:
	std::string itsMessage;

public:
	Exception(const std::string& msg);
	virtual ~Exception(void) throw();
	const char* what() const throw();
};

#define throw_SSL(msg) do {std::string s(__PRETTY_FUNCTION__); s = s + "; " + msg + "; "; throw SSLException(s); } while(0)

class SSLException: public Exception
{
public:
	SSLException(const std::string& msg) :
		Exception(msg + " SSL err: " + ERR_error_string(ERR_get_error(), 0))
	{
	}
	virtual ~SSLException(void) throw()
	{
	}
};
/**
 * @class MessageException
 * @brief Throwed by Message objects when they encounter problems, i.e. with
 * serializing/deserialzing
 */
class MessageException: public Exception
{
public:
	MessageException(const std::string& msg) : Exception(msg) {}
	virtual ~MessageException(void) throw() {};
};

/**
 * @class HttpException
 * @brief Throwed when http-related errors are encountered by Messages from
 * osock::http namespace
 */
class HttpException: public Exception
{
private:
	http::Status itsStatus;
public:
	HttpException(const std::string& msg, const http::Status& status);
	virtual ~HttpException(void) throw();
    const http::Status& getStatus() const;
};

/**
 * @class StdException
 * @brief Throwed by low level wrappers, to inform about problems in lowlevel,
 * standard routines (read/write/close etc)
 */
class StdException: public Exception
{
public:
	StdException(const std::string& msg) :
		Exception(msg +"; errno="+ strerror(errno))
	{
	}

	virtual ~StdException(void) throw()
	{
	}
};

/**
 * @class ArgsException
 * @brief Throwed when bad arguments were provided
 */
class ArgsException: public Exception
{
public:
	ArgsException(const std::string& msg) :
		Exception(msg)
	{
	}
	virtual ~ArgsException(void) throw()
	{
	}
};

} //namespace osock

#endif /* EXCEPTIONS_H_ */

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

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <openssl/err.h>
#include <errno.h>
#include <iostream>
#include "../defines.h"

class Exception
{
private:
	std::string itsMessage;
	int itsError;

public:
	Exception(const std::string& msg, int error = 0) :
		itsMessage(msg), itsError(error)
	{
		DBG_DESTRUCTOR;
		WRN << itsMessage << std::endl;
	}
	virtual ~Exception(void)
	{
		DBG_CONSTRUCTOR;
	}

	int GetError(void) const
	{
		return itsError;
	}
	virtual void Print(void) const;
};

#define throw_SSL(msg) do {std::string s(__PRETTY_FUNCTION__); s = s + "; " + msg + "; "; throw SSLException(s); } while(0)

class SSLException: public Exception
{
public:
	SSLException(const std::string& msg) :
		Exception(msg + " SSL err: " + ERR_error_string(ERR_get_error(), 0), 0)
	{
	}
	virtual ~SSLException(void)
	{
	}
};

class StdException: public Exception
{
public:
	StdException(const std::string& msg) :
		Exception(msg, errno)
	{
	}
	virtual ~StdException(void)
	{
	}
};

class NetException: public Exception
{
public:
	NetException(const std::string& M) :
		Exception(M, errno)
	{
	}
	virtual ~NetException(void)
	{
	}
};

#endif /* EXCEPTIONS_H_ */

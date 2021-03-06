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

#ifndef OSOCKSECURITY_H_
#define OSOCKSECURITY_H_

#include "../Address/Address.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

namespace osock
{
//! Abstract Security base class
class Security
{
public:
	Security(BIO* bio = NULL);
	virtual ~Security();

	BIO* GetBIO() const;

protected:
	static bool libsslReady;
	BIO* itsBIO;

	void SetBIO(BIO* bio);
	static void libsslInit();
};
} //namespace osock

#endif /* SECURITY_H_ */

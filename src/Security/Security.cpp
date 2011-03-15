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

#include "Security.h"
#include "../defines.h"

#include <assert.h>

bool Security::libsslReady = false;

Security::Security(securityMode security) :
	itsBIO(NULL), itsSecurityMode(security)
{
	DBG_CONSTRUCTOR;
}

Security::~Security()
{
	DBG_DESTRUCTOR;
	BIO_free(itsBIO);
}

BIO* Security::GetBIO()
{
	if (!itsBIO) {
		itsBIO = PopulateBIO();
	}
	assert(itsBIO != NULL);

	return itsBIO;
}

SSL* Security::GetSSL()
{
	SSL* ssl = NULL;
	BIO_get_ssl(GetBIO(), &ssl);
	return ssl;
}

bool Security::isSafe()
{
	return (itsSecurityMode != securityNone);
}

void Security::libsslInit()
{
	//TODO: make it thread-safe
	if (!libsslReady) {
		ERR_load_BIO_strings();
		SSL_load_error_strings(); /* readable error messages */
		SSL_library_init(); /* initialize library */
		libsslReady = true;
	}
}

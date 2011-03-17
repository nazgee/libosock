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

#define DEBUG_WANTED

#include <defines.h>
#include <Security/Security.h>

#include <assert.h>

namespace osock
{
bool Security::libsslReady = false;

Security::Security(BIO* bio) :
	itsBIO(bio)
{
	DBG_CONSTRUCTOR;
}

Security::~Security()
{
	DBG_DESTRUCTOR;
	if (doNotCleanup) {
		DBG << "closing itsBIO was surpressed!" << std::endl;
		return;
	}

	if (GetSSL() != NULL) {
		DBG << "closing SSL itsBIO=" << itsBIO << std::endl;
		SSL_shutdown(GetSSL());
		SSL_free(GetSSL());
	} else {
		DBG << "closing raw itsBIO="<< itsBIO << std::endl;
		BIO_free_all(itsBIO);
	}
}

BIO* Security::GetBIO()
{
	if (!itsBIO) {
		SetBIO(PopulateBIO());
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

void Security::PreventBIOcleanup()
{
	doNotCleanup = true;
}

void Security::SetBIO(BIO* bio)
{
	itsBIO = bio;
}

BIO* Security::PopulateBIO()
{
	assert(itsBIO != NULL);
	return itsBIO;
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
}

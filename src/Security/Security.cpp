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

//

#include <Utilities/Logger.h>
#include <Security/Security.h>
#include <Utilities/SSLWrap.h>

#include <assert.h>

static osock::Logger logger("Security");

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

	SSLWrap::BIO_free_all(itsBIO);
	SetBIO(NULL);
}

BIO* Security::GetBIO() const
{
	assert(itsBIO != NULL);
	return itsBIO;
}

void Security::SetBIO(BIO* bio)
{
	itsBIO = bio;
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

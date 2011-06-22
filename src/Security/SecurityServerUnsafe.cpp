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

#include <defines.h>
#include <Utilities/Logger.h>
#include <Security/SecurityServerUnsafe.h>
#include <Exception/Exception.h>
#include <Utilities/SSLWrap.h>

static osock::Logger logger("SecurityServerUnsafe");

namespace osock
{
SecurityServerUnsafe::SecurityServerUnsafe(int listenPort) :
	SecurityServer(listenPort)
{
	std::string port = to_string(itsListenPort);
	BIO* bio = SSLWrap::BIO_new_accept(const_cast<char*>(port.c_str()));
	if (bio == NULL) {
		throw_SSL("BIO_new_accept failed");
	}

	DBG << "populated unsafe server BIO @port=" << itsListenPort << std::endl;
	SetBIO(bio);

	DBG_CONSTRUCTOR;
}

SecurityServerUnsafe::~SecurityServerUnsafe()
{
	DBG_DESTRUCTOR;
}

}

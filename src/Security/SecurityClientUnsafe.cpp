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
#include <Security/SecurityClientUnsafe.h>
#include <Exception/Exception.h>
#include <Utilities/SSLWrap.h>

namespace osock
{
SecurityClientUnsafe::SecurityClientUnsafe(Address& serverAddress) :
	SecurityClient(serverAddress)
{
	BIO* bio = SSLWrap::BIO_new_connect( const_cast<char *>(itsSrverAddress.GetHostAndPort().c_str()) );
	if(bio == NULL) {
		throw_SSL("BIO_new_connect failed");
	}
	DBG << "populated unsafe client BIO @host=" << itsSrverAddress.GetHostAndPort() << std::endl;
	SetBIO(bio);

	DBG_CONSTRUCTOR;
}

SecurityClientUnsafe::~SecurityClientUnsafe()
{
	DBG_DESTRUCTOR;
}

}

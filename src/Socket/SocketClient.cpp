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

#include "SocketClient.h"
#include "../Exception/Exception.h"
#include "../Security/SecurityClientSSL.h"
#include "../Security/SecurityClientUnsafe.h"

#include <netdb.h>

SocketClient::SocketClient(Address& serverAddress, SecurityClient* security):
	Socket((BIO*) NULL), itsSecurity(security)
{

	//TODO consider moving it somewhere else
	SetBIO( itsSecurity->GetBIO() );

	//TODO: maybe move connect out of here, and call it explicitly as one of methods?
	if(BIO_do_connect(GetBIO()) <= 0){
		throw_SSL("BIO_do_connect failed");
	}

	if ( itsSecurity->IsPeerVerified() ) {
		WRN << "server verification failed" <<  std::endl;
	} else {
		DBG << "server verification succeeded" << std::endl;
	}

	DBG_CONSTRUCTOR;
}

SocketClient::~SocketClient(void)
{
	DBG_DESTRUCTOR;
}

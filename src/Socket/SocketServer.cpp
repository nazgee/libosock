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

//#define DEBUG_WANTED

#include <defines.h>
#include <Socket/SocketServer.h>
#include <Exception/Exception.h>
#include <Security/SecurityServer.h>
#include <Security/SecurityServerSSL.h>
#include <Security/SecurityServerUnsafe.h>

#include <assert.h>
#include <arpa/inet.h>
#include <boost/scoped_ptr.hpp>

namespace osock
{
SocketServer::SocketServer(SecurityServer* security, serviceType type) :
		Socket(security), itsType(type), itsSecurityServer(security)
{
	/* first call of BIO_do_accpept does not really accept a connection-
	 * instead it does some initial-setup only */
	if (BIO_do_accept(GetBIO()) <= 0) {
		throw_SSL("BIO_do_accept failed");
	}
	DBG_CONSTRUCTOR;
}

SocketServer::~SocketServer(void)
{
	DBG_DESTRUCTOR;
}

void SocketServer::Accept(clientsHandler handler)
{
	BIO* client = AcceptIncoming();
	DBG << "serving new client ..." << std::endl;

	boost::scoped_ptr<Socket> socket(new Socket(client));
	switch (itsType) {
		case serviceCallback: {
			handler(*socket);
			DBG << "... client served" << std::endl;
			;
		}
			break;
		case serviceProcess:
		case serviceThread:
		default:
			assert(0);
	}
}

void SocketServer::Accept(Address& Addr, clientsHandler handler)
{
	BIO* client = AcceptIncoming();
	DBG << "serving new client ..." << std::endl;

	boost::scoped_ptr<Socket> socket(new Socket(client));

	//assign client address to given argument, so caller knows who had been served
	Addr = Address(client);

	switch (itsType) {
		case serviceCallback: {
			handler(*socket);
			DBG << "... client served" << std::endl;
		}
			break;
		case serviceProcess:
		case serviceThread:
		default:
			assert(0);
	}
}

BIO* SocketServer::AcceptIncoming()
{
	BIO* out;
	do
	{
		if (BIO_do_accept(GetBIO()) <= 0) {
			throw_SSL("BIO_do_accept failed");
		}

		out = BIO_pop(GetBIO());
		if (out == NULL) {
			throw_SSL("BIO_pop failed");
		}

		if (itsSecurityServer->DoHandshake(out)) {
			return out;
		}

		BIO_vfree(out);
		DBG << "DoHandshake failed, waiting for another client ("
			<< ERR_error_string(ERR_get_error(), 0) << ")" << std::endl;

	} while (1);

	return out;
}
}

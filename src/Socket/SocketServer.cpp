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
#include <Socket/SocketServer.h>
#include <Exception/Exception.h>
#include <Security/SecurityServer.h>
#include <Security/SecurityServerSSL.h>
#include <Security/SecurityServerUnsafe.h>
#include <Utilities/SSLWrap.h>
#include <Server/Server.h>

#include <assert.h>
#include <arpa/inet.h>

static osock::Logger logger("SocketServer");

namespace osock
{
SocketServer::SocketServer(SecurityServer* security, serviceType type) :
		Socket(security), itsSecurityServer(security),
		isForked(false), itsType(type)
{
	// First call of BIO_do_accpept does not really accept a connection-
	// instead it does some initial-setup only
	if (SSLWrap::BIO_do_accept_(GetBIO()) <= 0) {
		throw_SSL("BIO_do_accept failed");
	}
	DBG_CONSTRUCTOR;
}

SocketServer::~SocketServer(void)
{
	DBG_DESTRUCTOR;
}

void SocketServer::Accept(Address& Addr, clientsHandler handler, Server* controller)
{
	while (1) {
		BIO* client = AcceptIncoming();
		// Assign client address, so caller knows who had been served
		Addr = Address(client);

		DBG << "serving new client: " << Addr << std::endl;

		switch (itsType) {
			case serviceCallback: {
				// Perform handshake to decide whether client is authorized
				BIO* auth = itsSecurityServer->DoHandshake(client);
				if (auth == NULL) {
					DBG << "Handshake with " << Addr << " failed"
							<< std::endl;
					SSLWrap::BIO_free_all(client);
					// Let's wait for another client
					break;
				}

				// auth BIO will be cleaned up when socket goes out of scope
				Socket socket(auth);
				// Call handler to serve client
				if (controller != NULL)
					controller->Serve(socket);
				else
					handler(socket);

				DBG << "Client: " << Addr << " served" << std::endl;
				return;
			} break;
			case serviceProcess: {
				int childpid = fork();
				if (childpid < 0)
					throw StdException("Error while forking!");

				if (childpid == 0) {
					// We have to make sure that parent's BIO will not be shuted
					// down when we (child) will be cleaning up
					if (SSLWrap::BIO_set_close_(GetBIO(), BIO_NOCLOSE) != 1)
						throw_SSL("BIO_set_close failed!");

					// Close fd, so we are not waisting descriptors
					int fd = -1;
					BIO_get_fd(GetBIO(), &fd);
					close(fd);

					SetForked(true);

					// Perform handshake to decide whether client is authorized
					BIO* auth = itsSecurityServer->DoHandshake(client);
					if (auth == NULL) {
						DBG << "Handshake with " << Addr << " failed"
								<< std::endl;
						SSLWrap::BIO_free_all(client);
						// Let's wait for another client
						break;
					}

					// auth BIO will be cleaned up when socket goes out of scope
					Socket socket(auth);
					// Call handler to serve client
					if (controller != NULL)
						controller->Serve(socket);
					else
						handler(socket);

					DBG << "Client: " << Addr << " served" << std::endl;
					return;
				} else {
					// We have to make sure that child's BIO will not be shuted
					// down when we (parent) will be cleaning up
					if (SSLWrap::BIO_set_close_(client, BIO_NOCLOSE) != 1)
						throw_SSL("BIO_set_close failed!");

					// Close fd, so we are not waisting descriptors
					int fd = -1;
					BIO_get_fd(client, &fd);
					close(fd);

					DBG << "Getting back to accepting clients" << std::endl;
					SSLWrap::BIO_free_all(client);
					return;
				}
			} break;
			case serviceThread: {
#if defined(OPENSSL_THREADS)
				// Thread support enabled
				assert(0); //TODO implement it
#else
				// No thread support
				assert(0);
#endif
			} break;
			default:
				assert(0);
		}
	}
}

bool SocketServer::IsMainInstance()
{
	DBG << "isForked=" << isForked << std::endl;
	return !isForked;
}

void SocketServer::SetForked(bool forked)
{
	DBG << "isForked=" << isForked << "->" << forked << std::endl;
	isForked = forked;
}

BIO* SocketServer::AcceptIncoming()
{
	BIO* out;

	// Sit and wait on our accept channel
	if (SSLWrap::BIO_do_accept_(GetBIO()) <= 0) {
		throw_SSL("BIO_do_accept failed");
	}

	// Pop out incoming client from our accept channel
	out = SSLWrap::BIO_pop(GetBIO());
	if (out == NULL) {
		throw_SSL("BIO_pop failed");
	}

	return out;
}

}

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

#include <wait.h>
#include <signal.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <Server/Server.h>
#include <Utilities/Logger.h>
#include <Utilities/Utils.h>

static osock::Logger logger("Server");

namespace osock
{

Server::Server(Auth_p auth, std::string portname, serviceType servicetype) :
		itsAuth(auth),
		itsBIO(BIOSocket::PopulateAcceptBIO(portname)),
		itsServiceType(servicetype),
		isChild(false),
		itsPortName(portname)
{
	DBG_CONSTRUCTOR;
}

Server::~Server()
{
	DBG_DESTRUCTOR;
//	delete itsSocketServer;
}

bool Server::onServed(Address& servedClient)
{
	DBG << "Serving " << servedClient << " done! Waiting for next client." << std::endl;
	return true;
}

void Server::doRunCallback(BIO_p client)
{
	// Perform handshake to decide whether client is authorized
	BIO_p auth = itsAuth->Authenticate(client);
	Manage(auth);

	DBG << "Client served (callback)" << std::endl;
}

void Server::doRunProcess(BIO_p client)
{
	int childpid = fork();
	if (childpid < 0)
		throw StdException("Error while forking!");

	if (childpid == 0) {
		isChild = true;
		// We have to make sure that parent's BIO will not be shuted
		// down when we (child) will be cleaning up
		itsBIO->setClose(false);

		// Close fd manually, so we are not leaking descriptors
		close(itsBIO->getFD());

		// Perform handshake to decide whether client is authorized
		BIO_p auth = itsAuth->Authenticate(client);
		//FIX: this check is not needed
		if (auth) {
			Manage(auth);
		} else {
			NFO << "Client not authorized (fork)" << std::endl;
		}

		DBG << "Client served (fork)" << std::endl;
	} else {
		// We have to make sure that child's BIO will not be shuted
		// down when we (parent) will be cleaning up
		client->setClose(false);
		// Close fd manually, so we are not leaking descriptors
		close(client->getFD());

		DBG << "Getting back to accepting clients (fork)" << std::endl;
	}
}

void Server::serverThread(BIO_p client)
{
	// Perform handshake to decide whether client is authorized
	BIO_p auth = itsAuth->Authenticate(client);
	Manage(auth);

	ERR << "Client served (thread)" << std::endl;
}

void Server::doRunThread(BIO_p client)
{
#if defined(OPENSSL_THREADS)
	boost::thread thread( boost::bind(&Server::serverThread, this, client));
#else
	// No thread support
	assert(0);
#endif
}

void Server::Start()
{
	Address client;
	do {
		DBG << "Server is starting to accept clients" << std::endl;
		BIOSocket_p client = itsBIO->AcceptIncoming();

		try {
			switch (itsServiceType) {
				case serviceCallback: {
					doRunCallback(client);
				} break;
				case serviceProcess: {
					doRunProcess(client);
				} break;
				case serviceThread: {
					doRunThread(client);
				} break;
				default:
					assert(0);
			}
		} catch (BIO::RemoteDiedException& e) {
		}
	} while(!isChild && onServed(client));

	if (isChild) {
		exit(0);
	}
}

void Server::ChildReaper(int n)
{
	wait3(NULL, WNOHANG, NULL);
	NFO_FUNC << "I'd just ripped one of your children- bye bye, daddy!" << std::endl;
}

void Server::InstallChildReaper(void (*reaper)(int))
{
	// Prevent zombies creation
	signal(SIGCHLD, reaper);
}

} //namespace osock

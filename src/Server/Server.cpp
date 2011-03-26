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


#include <Socket/SocketServer.h>
#include <Server/Server.h>
#include <Utilities/Logger.h>
#include <defines.h>

namespace osock
{

Server::Server(SocketServer* socketServer) :
		itsSocketServer(socketServer)
{
	DBG_CONSTRUCTOR;
}

Server::~Server()
{
	DBG_CONSTRUCTOR;
	delete itsSocketServer;
}

bool Server::ClientServed(Address& servedClient)
{
	DBG << "Serving " << servedClient << " done! Waiting for next client." << std::endl;
	return true;
}

void Server::Handle(const Socket& Client)
{
	WRN << "Default handler for client=" << Client << ", doing nothing!"
			<< std::endl;
}

void Server::Run()
{
	Address client;
	do {
		itsSocketServer->Accept(client, NULL, this);
	} while (itsSocketServer->IsMainInstance() && ClientServed(client));

	if (!itsSocketServer->IsMainInstance()) {
		delete itsSocketServer;
		DBG << "Serving " << client << " done!" << std::endl;
		exit(0);
	}
}

} //namespace osock

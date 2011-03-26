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


#ifndef SERVER_H_
#define SERVER_H_

#include "../Socket/Socket.h"
#include "../Address/Address.h"

namespace osock
{
class SocketServer;
class Server
{
public:
	Server(SocketServer* socketServer);
	virtual ~Server();
	void Run();

	virtual bool ClientServed(Address& servedClient);
	virtual void Handle(const Socket& Client);
private:
	SocketServer* itsSocketServer;
};

}

#endif /* SERVER_H_ */

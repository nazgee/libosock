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

#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

#include "Socket.h"
#include "../Security/SecurityServer.h"

namespace osock
{
class SocketServer: public Socket
{
public:
	typedef enum { serviceCallback, serviceProcess, serviceThread } serviceType;
	typedef void (*clientsHandler)(const Socket& Client);

	SocketServer(SecurityServer* security, serviceType type = serviceCallback);
	virtual ~SocketServer(void);

	//! Accept client connection and call \a handler to deal with it; peer adress is stored in \a client
	virtual void Accept(Address& client, clientsHandler handler);
	//! Checks whether this server instance is main- or forked-one.
	bool IsMainInstance();

private:
	SecurityServer* itsSecurityServer;
	bool isForked;

protected:
	serviceType itsType;
	void SetForked(bool forked);
	//! Waits for client to connect \return BIO* of the connected client
	BIO* AcceptIncoming();
};
} //namespace osock

#endif /* SOCKETSERVER_H_ */

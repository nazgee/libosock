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

#ifndef SOCKETSERVERUNSAFE_H_
#define SOCKETSERVERUNSAFE_H_

#include "SocketServer.h"

namespace osock
{
class SecurityServerUnsafe;
class SocketServerUnsafe: public SocketServer
{
public:
	SocketServerUnsafe(	int port, serviceType type = serviceCallback);
	virtual ~SocketServerUnsafe();
protected:
	SecurityServerUnsafe* itsSecurityServerUnsafe;
};
} //namespace osock

#endif /* SOCKETSERVERUNSAFE_H_ */

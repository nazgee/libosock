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

#ifndef SOCKET_H_
#define SOCKET_H_

#include "../Address/Address.h"
#include "../Message/Message.h"

#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <iostream>

namespace osock
{
class Security;
class Socket
{
private:
	int itsSD;
	Security* itsSecurity;

protected:
	BIO* GetBIO() const;
	int GetSD() { return itsSD; }

public:
	Socket(BIO* bio);
	//! \a security will be destroyed when ~Socket will be executed
	Socket(Security* security);
	Socket(Socket& sock);
	virtual ~Socket(void);

	int Send(Message& Msg, int Options = 0) const;
	int Receive(Message& Msg, int Options = 0) const;

	friend std::ostream& operator <<(std::ostream &os, const Socket &obj);
	friend std::ostream& operator <<(std::ostream &os, const Socket *obj);
};
} //namespace osock

#endif /* SOCKET_H_ */

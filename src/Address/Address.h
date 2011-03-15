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

#ifndef HOSTADDRESS_H_
#define HOSTADDRESS_H_

#include "../defines.h"
#include <stdint.h>
#include <string>
#include <netdb.h>
#include <openssl/bio.h>
#include <boost/scoped_array.hpp>

///////////////////////////////////////////////////////////////////////////////
/// Address
class Address
{
private:
	struct addrinfo * itsAddrInfo;

	int itsPort;
	int itsBufferSize;
	boost::scoped_array<char> itsBuffer;
	char* itsHostname;
	char* itsPortname;

	static void GetNameInfo(struct addrinfo *ai, char* hbuffer, int hbuff_size,
			char* pbuffer, int pbuff_size);
	static void GetAddrInfo(char* host, char* port, struct addrinfo **ai);

public:
	//TODO create default constructor- in some situations we might need a default Address, to which other instance Address will be assigned later
	Address(const char* Name);
	Address(BIO* bio);
	Address(const Address& other);
	Address& operator=(const Address& other);
	virtual ~Address(void);

	uint16_t GetPortFormatNet(void) const;
	uint16_t GetPortFormatHost(void) const;
	struct sockaddr* GetAddr(void) const;
	socklen_t GetAddrlen(void) const;

	std::string GetHostAndPort(bool rebuild = false);

	friend std::ostream& operator <<(std::ostream &os, const Address &obj);
	friend std::ostream& operator <<(std::ostream &os, const Address *obj);
};

#endif /* HOSTADDRESS_H_ */

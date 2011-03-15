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

#include <Address/Address.h>
#include <Exception/Exception.h>

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <stdio.h>

void Address::GetAddrInfo(char* host, char* port, struct addrinfo **ai)
{
	int error = getaddrinfo(host, port, NULL, ai);
	if (error != 0) {
		std::string errorstring = "error in getaddrinfo:";
		errorstring += gai_strerror(error);
		throw NetException(errorstring);
	}

//	DBG << "host=" << host << " port=" << port << " ai=" << (void*) *ai
//			<< std::endl;
}

void Address::GetNameInfo(struct addrinfo *ai, char* hbuffer, int hbuff_size,
		char* pbuffer, int pbuff_size)
{
	int error = getnameinfo(ai->ai_addr, ai->ai_addrlen, hbuffer, hbuff_size,
			pbuffer, pbuff_size, 0);
	if (error != 0) {
		std::string errorstring = "error in getnameinfo:";
		errorstring += gai_strerror(error);
		throw NetException(errorstring);
	}

//	DBG << " ai=" << (void*) ai << " hbuffer=" << hbuffer << " hbuff_size="
//			<< hbuff_size << " pbuffer=" << pbuffer << " pbuff_size="
//			<< pbuff_size << std::endl;
}

Address::Address(const char* Name) :
	itsAddrInfo(NULL), itsBufferSize(strlen(Name) + 1),
			itsBuffer(new char[itsBufferSize]), itsHostname(itsBuffer.get())
{
	//prepare copy of hostname- a bufffer on which we can freely operate
	strcpy(itsHostname, Name);

	//make sure we got something in form "host:port"
	itsPortname = strrchr(itsHostname, ':');
	if (itsPortname != 0 && (itsPort = atoi(itsPortname + 1)) > 0) {
		*itsPortname = 0;
		itsPortname++;
	} else {
		throw Exception("Host name should be in format \"host:port\"", EINVAL);
	}

	//prepare addrinfo structure- we will use it during this object lifetime
	GetAddrInfo(itsHostname, itsPortname, &itsAddrInfo);

	DBG_CONSTRUCTOR;
}

Address::Address(BIO* bio) :
	itsAddrInfo(NULL), itsPort(-1),
	itsBufferSize(0),
	itsBuffer(NULL), itsHostname(NULL),
	itsPortname(NULL)
{
	if (bio == NULL)
		return;

	//translate socket to "host:port" string
	int sock = BIO_get_fd(bio, NULL);
	struct sockaddr peer;
	socklen_t peer_len = sizeof(peer);
	if (getpeername(sock, &peer, &peer_len) == -1) {
		throw NetException("getpeername failed");
	}

	char hbuff[256];
	char pbuff[32];
	int error = getnameinfo(&peer, peer_len, hbuff, sizeof(hbuff), pbuff,
			sizeof(pbuff), 0);
	if (error != 0) {
		std::string errorstring = "error in getnameinfo:";
		errorstring += gai_strerror(error);
		throw NetException(errorstring);
	}

	itsBufferSize = strlen(hbuff) + strlen(pbuff) + 2; //"host:port"
	itsBuffer.reset(new char[itsBufferSize]);
	itsHostname = itsBuffer.get();

	//prepare copy of hostname- a bufffer on which we can freely operate
	strcpy(itsHostname, hbuff);
	itsHostname[strlen(hbuff)] = ':';
	strcpy(&itsHostname[strlen(hbuff) + 1], pbuff);

	//make sure we got something in form "host:port"
	itsPortname = strrchr(itsHostname, ':');
	if (itsPortname != 0 && (itsPort = atoi(itsPortname + 1)) > 0) {
		*itsPortname = 0;
		itsPortname++;
	} else {
		throw Exception("Host name should be in format \"host:port\"", EINVAL);
	}

	//prepare addrinfo structure- we will use it during this object lifetime
	GetAddrInfo(itsHostname, itsPortname, &itsAddrInfo);

	DBG_CONSTRUCTOR;
}

Address::Address(const Address& other) :
	itsAddrInfo(NULL), itsPort(other.itsPort),
	itsBufferSize(other.itsBufferSize),
	itsBuffer(new char[itsBufferSize]), itsHostname(itsBuffer.get()),
	itsPortname(itsHostname + (other.itsPortname - other.itsHostname))
{
	memcpy(itsBuffer.get(), other.itsBuffer.get(), itsBufferSize);

	//prepare addrinfo structure- we will use it during this object lifetime
	GetAddrInfo(itsHostname, itsPortname, &itsAddrInfo);

	DBG_CONSTRUCTOR;
}



Address& Address::operator=(const Address& other)
{
	// 1: allocate new memory and copy the elements
	boost::scoped_array<char> newBuffer (new char[other.itsBufferSize]);
	memcpy(newBuffer.get(), other.itsBuffer.get(), other.itsBufferSize);

	// 2: deallocate old memory
	freeaddrinfo(itsAddrInfo);

	// 3: assign the new memory to the object
	itsBufferSize = other.itsBufferSize;
	itsPort = other.itsPort;
	itsBuffer.swap(newBuffer);
	itsHostname = itsBuffer.get();
	itsPortname = itsHostname + (other.itsPortname - other.itsHostname);
	GetAddrInfo(itsHostname, itsPortname, &itsAddrInfo);

	DBG << this << std::endl;
	return *this;
}

Address::~Address(void)
{
	DBG_DESTRUCTOR;
	freeaddrinfo(itsAddrInfo);
}

uint16_t Address::GetPortFormatNet(void) const
{
	return htons(GetPortFormatHost());
}

uint16_t Address::GetPortFormatHost(void) const
{
	return itsPort;
}

struct sockaddr* Address::GetAddr(void) const
{
	return itsAddrInfo->ai_addr;
}
socklen_t Address::GetAddrlen(void) const
{
	return itsAddrInfo->ai_addrlen;
}

std::string Address::GetHostAndPort(bool rebuild)
{
	std::string s;
	if (rebuild) {
		char hostbuff[256];
		char portbuff[32];
		GetNameInfo(itsAddrInfo, hostbuff, sizeof(hostbuff), portbuff,
				sizeof(portbuff));
		s += hostbuff;
		s += ":";
		s += portbuff;
	} else {
		s += itsHostname;
		s += ":";
		s += itsPortname;
	}
	DBG << "rebuild=" << rebuild << " return=" << s << std::endl;

	return s;
}

std::ostream& operator <<(std::ostream &os, const Address *obj)
{
	if (obj->itsAddrInfo == NULL)
		os << (void*) obj << "<undefined address>";
	else
		os << (void*) obj << " " << obj->itsHostname << ":" << obj->itsPort;

	return os;
}

std::ostream& operator <<(std::ostream &os, const Address &obj)
{
	return operator<<(os, &obj);
}

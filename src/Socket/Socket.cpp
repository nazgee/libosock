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

#include <Socket/Socket.h>
#include <defines.h>
#include <Exception/Exception.h>
#include <Security/Security.h>

#include <assert.h>
#include <arpa/inet.h>

#include <boost/scoped_array.hpp>

//TODO: create it in a more intelligent manner, maybe in a dedicated file?
NullStream nullstream;

Socket::Socket(BIO* bio) :
	itsSD(-1),
	itsBIO(NULL),
	itsSecurity(NULL)
{
	SetBIO(bio);

	DBG_CONSTRUCTOR;
}

Socket::Socket(Security* security) :
	itsSD(-1),
	itsBIO(NULL),
	itsSecurity(security)
{
	assert(itsSecurity != NULL);
	SetBIO(itsSecurity->GetBIO());

	DBG_CONSTRUCTOR;
}

Socket::Socket(Socket& sock)
{
	int err;
	socklen_t size=sizeof(err);

	if ( getsockopt(itsSD, SOL_SOCKET, SO_ERROR, &err, &size) != 0 )
		throw NetException("Socket error");
	if ( (itsSD = dup(sock.itsSD)) < 0 )
		throw StdException("Can't copy socket");

	DBG_CONSTRUCTOR;
}

Socket::~Socket(void)
{
	DBG_DESTRUCTOR;
	//whole BIO chain has to be fred, in order to prevent memoryleak
	BIO_free_all(itsBIO);
}

void Socket::SetBIO(BIO* bio)
{
	itsBIO = bio;
	if (itsBIO != NULL)
		itsSD = BIO_get_fd(itsBIO, NULL);
	else
		itsSD = -1;
}

int  Socket::Send(Message& Msg, int Options) const
{
	int bytes = 0;
	boost::scoped_array<char> buf(Msg.Unpack(bytes));

	int i, off = 0;
	for (;;) {
		DBG << "sending " << bytes << "B" << std::endl;
		i = BIO_write(itsBIO, &buf[off], bytes);
		if (i <= 0) {
			if (BIO_should_retry(itsBIO)) {
				DBG << "sleeping before retrying write" << std::endl;
				sleep(1);
				continue;
			}
			throw_SSL("BIO_write failed in unretryable way");
		}
		off += i;
		bytes -= i;
		if (bytes <= 0)
			break;
	}

	return bytes;
}

int  Socket::Receive(Message& Msg, int Options) const
{
	DBG << "waiting for message" << std::endl;
	bool done;
	int bytes, cnt=0;
	static const int bufsize = 8;	//TODO make it bigger or even modifiable

	boost::scoped_array<char> buf(new char[bufsize]);

	do {
		bytes = BIO_read(itsBIO, buf.get(), bufsize);
		if (bytes == 0) {
			throw_SSL("BIO_read returned 0 - client disconnected");
		}

		if (bytes < 0) {
			if (BIO_should_retry(itsBIO)) {
				DBG << "sleeping before retrying read" << std::endl;
				//XXX is sleeping here really needed?!
				sleep(1);
				continue;
			}
			throw_SSL("BIO_read failed in unretryable way");
		}
		cnt++;
		DBG << "received " << bytes << "B in chunk=" << cnt<< std::endl;
		done = Msg.Pack(buf.get(), bytes, cnt);
	} while ( !done );

	return bytes;
}

std::ostream& operator <<(std::ostream &os,const Socket *obj)
{
	os << (void*)obj << " itsBIO=" << obj->itsBIO << " itsSD=" << obj->itsSD;
	return os;
}

std::ostream& operator <<(std::ostream &os,const Socket &obj)
{
	return operator<<(os, &obj);
}

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

#include <defines.h>
//#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Logger.h>
#include <Utilities/Utils.h>
#include <Socket/Socket.h>
#include <Exception/Exception.h>
#include <Security/Security.h>
#include <Utilities/SSLWrap.h>

#include <errno.h>
#include <assert.h>
#include <arpa/inet.h>
#include <boost/scoped_array.hpp>
#include <algorithm>

static osock::Logger logger("Socket");

namespace osock
{
Socket::Socket(BIO* bio) :
	itsSD(-1), itsSecurity(new Security(bio))
{
	BIO_get_fd(GetBIO(), &itsSD);
	DBG_CONSTRUCTOR;
}

Socket::Socket(Security* security) :
	itsSD(-1), itsSecurity(security)
{
	assert(itsSecurity != NULL);
	BIO_get_fd(GetBIO(), &itsSD);

	DBG_CONSTRUCTOR;
}

Socket::Socket(Socket& sock)
{
	int err;
	socklen_t size = sizeof(err);

	if (getsockopt(itsSD, SOL_SOCKET, SO_ERROR, &err, &size) != 0)
		throw StdException("Socket error");
	if ((itsSD = dup(sock.itsSD)) < 0)
		throw StdException("Can't copy socket");

	DBG_CONSTRUCTOR;
}

Socket::~Socket(void)
{
	DBG_DESTRUCTOR;
	delete itsSecurity;
}

BIO* Socket::GetBIO() const
{
	return itsSecurity->GetBIO();
}

int Socket::Send(Message& message) const
{
	data_chunk buf(message.Unpack());

	int bytes = buf.size();
	int i, off = 0;
	for (;;) {
		DBG << "sending " << bytes << "B" << std::endl;
		BIO* b = GetBIO();
		i = BIO_write(b, &buf[off], bytes);
		if (i <= 0) {
			if (BIO_should_retry(GetBIO())) {
				WRN << "sleeping before retrying write" << std::endl;
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

int Socket::Receive(Message& message, int timeout_ms)
{
	static const int chunkSize = 16; //TODO make it bigger or even better: modifiable
	int rxedNumber = 0;
	data_chunk tempData(chunkSize);

	DBG << "Getting message ready for Pack()" << std::endl;
	message.RestartPacking();
	DBG << "Remains from previous read:" << Utils::DataToString(itsRemainsOfData)
			<< std::endl;

	SSLWrap::BIO_set_read_tmo(GetBIO(), timeout_ms);
	if (!message.Pack(itsRemainsOfData)) {
		DBG << "Waiting for message" << std::endl;
		do {
			//XXX: optimize this loop
			tempData.resize(chunkSize);
			rxedNumber = BIO_read(GetBIO(), &tempData[0], chunkSize);
			if (rxedNumber == 0) {
				throw_SSL("BIO_read returned 0 - client disconnected");
			}

			if (rxedNumber < 0) {
				if (BIO_should_retry(GetBIO())) {
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
						throw Exception("Read timeout!");
					}

					NFO << "Sleeping before retrying; ret="
							<< to_string(rxedNumber) << "errno="
							<< to_string(errno) << std::endl;
					sleep(1); //XXX is sleeping here really needed?!
					tempData.clear();
					continue;
				}
				throw_SSL("BIO_read failed in unretryable way");
			}
			//only bytes that were rxed should be assumed as valid
			tempData.resize(rxedNumber);
			DBG << "data_RX" << Utils::DataToString(tempData) << std::endl;

			//try to Pack() data that was just received
		} while (!message.Pack(tempData));
	}

	// Store remainings for future use
	itsRemainsOfData = message.getRemains();
	DBG << "DONE! Remains from current read:" << Utils::DataToString(
			itsRemainsOfData) << std::endl;

	return rxedNumber;
}

std::ostream& operator <<(std::ostream &os, const Socket *obj)
{
	os << (void*) obj << " itsBIO=" << obj->GetBIO() << " itsSD=" << obj->itsSD;
	return os;
}

std::ostream& operator <<(std::ostream &os, const Socket &obj)
{
	return operator<<(os, &obj);
}
}

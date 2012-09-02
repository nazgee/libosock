/*
 * SocketBIO.cpp
 *
 *  Created on: Jun 28, 2011
 *      Author: nazgee
 */

#include <BIO/BIOSocket.h>
#include <Utilities/Logger.h>
static osock::Logger logger("BIOSocket");

namespace osock
{

BIOSocket_p BIOSocket::PopulateAcceptBIO(std::string portname, unsigned int chunksize)
{
	DBG_FUNC << "Populating accept BIO; port=" << portname << std::endl;
	::BIO* b;
	b = ::BIO_new_accept(const_cast<char*>(portname.c_str()));
	if (b == NULL) {
		throw StdException("Creation of new accept BIO failed");
	}
	BIOSocket_p ret(new BIOSocket(b, chunksize));

	// First call of BIO_do_accpept does not really accept a connection-
	// instead it does some initial-setup only
	if ( BIO_do_accept(b) <= 0 ) {
		throw StdException("Accept BIO init failed");
	}
	return ret;
}

BIOSocket_p BIOSocket::PopulateClientBIO(std::string hostname, unsigned int chunksize)
{
	DBG_FUNC << "Populating client BIO; host=" << hostname << std::endl;
	::BIO* b;
	b = ::BIO_new_connect(const_cast<char*>(hostname.c_str()));
	if (b == NULL) {
		throw StdException("Creation of new connect BIO failed");
	}
	BIOSocket_p ret(new BIOSocket(b, chunksize));

	while (BIO_do_connect(b) <= 0) {
		if (!BIO_should_retry(b)) {
			throw StdException("Connecting to server failed");
		}
	}

	return ret;
}

BIOSocket::BIOSocket(::BIO* bio, bool closedescriptor, unsigned int chunksize) :
	BIO(bio, closedescriptor, chunksize)
{
	DBG_CONSTRUCTOR;
}

BIOSocket::~BIOSocket()
{
	DBG_DESTRUCTOR;
}

void BIOSocket::setOption(int level, int optname, void* optvalue,
		socklen_t optlen)
{
	if (setsockopt(getFD(), level, optname, optvalue, optlen)) {
		throw StdException("Setting socket option failed");
	}
}

void BIOSocket::setReadTimeout(int ms)
{
	// XXX fix values smaller than 1000!
	struct timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = (ms - tv.tv_sec * 1000) / 1000;

	setOption(SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
}

void BIOSocket::setWriteTimeout(int ms)
{
	struct timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = (ms - tv.tv_sec * 1000) / 1000;

	setOption(SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval));
}

boost::shared_ptr<BIOSocket> BIOSocket::AcceptIncoming()
{
	::BIO* out;

	// Sit and wait on our accept channel
	if (BIO_do_accept(getBIO()) <= 0) {
		throw StdException("Accepting client failed");
	}

	// Pop out incoming client from our accept channel
	out = BIO_pop(getBIO());
	if (out == NULL) {
		throw StdException("Popping client BIO failed");
	}

	boost::shared_ptr<BIOSocket> ret(new BIOSocket(out, getChunkSize()));
	return ret;
}

}

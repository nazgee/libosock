/*
 * AuthSSL.cpp
 *
 *  Created on: Jul 3, 2011
 *      Author: nazgee
 */

#include <Security/AuthSSL.h>
#include <BIO/BIOSSL.h>
#include <Utilities/Logger.h>

static osock::Logger logger("AuthSSL");

namespace osock
{

AuthSSL::AuthSSL(bool clientmode, SSLContext_p context) :
	isClient(clientmode), itsContext(context)
{
	DBG_CONSTRUCTOR;
}

BIO_p AuthSSL::Authenticate(BIO_p bio)
{
	DBG << "Authenticating remote" << (isClient ? "server" : "client") << std::endl;

	BIOSSL_p BIO_ssl;
	if (isClient) {
		BIO_ssl = BIOSSL::PopulateClientBIOSSL(itsContext);
	} else {
		BIO_ssl = BIOSSL::PopulateServerBIOSSL(itsContext);
	}
	// Install SSL BIO on top of BIO to authenticate
	BIO_ssl->pushBIO(bio);

	if (!BIO_ssl->Handshake()) {
		// In case of handshake failure, remove SSL BIO from our BIO ...
		BIO_ssl->popBIO();
		// ... and return NULL
		throw_SSL("SSL Handshaking remote failed");
	}
	return BIO_ssl;
}

AuthSSL_p AuthSSL::PopulateClientAuth(SSLContext_p context)
{
	AuthSSL_p ret(new AuthSSL(true, context));
	return ret;
}

AuthSSL_p AuthSSL::PopulateServerAuth(SSLContext_p context)
{
	AuthSSL_p ret(new AuthSSL(false, context));
	return ret;
}

}

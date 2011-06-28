/*
 * BIOSSL.cpp
 *
 *  Created on: Jul 3, 2011
 *      Author: nazgee
 */

#include <BIO/BIOSSL.h>
#include <Utilities/Logger.h>
static osock::Logger logger("BIOSSL");

namespace osock {

BIOSSL::BIOSSL(::BIO* bio, bool closedescriptor, unsigned int chunksize) :
		BIO(bio, closedescriptor, chunksize)
{
	DBG_CONSTRUCTOR;

	if(this->getSSL() == NULL) {
		throw_SSL("Given BIO is not SSL BIO");
	}

	// We don't want any retries
	SSL_set_mode(this->getSSL(), SSL_MODE_AUTO_RETRY);
}

BIOSSL_p BIOSSL::PopulateClientBIOSSL(SSLContext_p context)
{
	DBG_FUNC << "Populating client SSL BIO based on CTX=" << context << std::endl;
	BIOSSL_p ret(new BIOSSL(BIO_new_ssl(*context, 1)));

	return ret;
}

BIOSSL_p BIOSSL::PopulateServerBIOSSL(SSLContext_p context)
{
	DBG_FUNC << "Populating server SSL BIO based on CTX=" << context << std::endl;
	BIOSSL_p ret(new BIOSSL(BIO_new_ssl(*context, 0)));

	return ret;
}

::SSL* BIOSSL::getSSL()
{
	SSL* ssl = NULL;
	BIO_get_ssl(this->getBIO(), &ssl);

	return ssl;
}

bool BIOSSL::Handshake()
{
	do {
		if (BIO_do_handshake(this->getBIO()) <= 0) {
			if (!this->ShouldRetry()) {
				//handshake failed, and retry won't help
				WRN << "SSL handshake failed " << ERR_error_string(ERR_get_error(), 0) << std::endl;
				return false;
			} else {
				//handshake failed, but there is a chance it will succeed on retry
				DBG << "Retrying SSL handshake" << std::endl;
			}
		} else {
			DBG << "SSL handshake succeeded" << std::endl;
			return true;
		}
	} while(1);

}

}

/*
 * Context.cpp
 *
 *  Created on: Jul 3, 2011
 *      Author: nazgee
 */

#include <Security/SSLContext.h>
#include <Utilities/Logger.h>
#include <Exception/Exception.h>

static osock::Logger logger("SSLContext");


namespace osock {
SSLContext::SSLContext(	securityMode method,
				std::string certFile,
				std::string keyFile,
				std::string trustFile,
				std::string password) :
	itsSecurityMode(method),
	itsCertFile(certFile),
	itsKeyFile(keyFile),
	itsTrustFile(trustFile),
	itsPassword(password)
{
	DBG_CONSTRUCTOR;
	// Initialize SSL library, if it was not done before
	libsslInit();

	// Create SSL context
#ifdef LIBSSL_WITHOUT_CONST_PARAMS
	SSL_CTX* tmp = SSL_CTX_new(GetMethod(itsSecurityMode));
#else
	SSL_CTX* tmp = SSL_CTX_new(const_cast<SSL_METHOD *>(GetMethod(itsSecurityMode)));
#endif
	if (tmp == NULL) {
		throw_SSL("SSL_CTX_new failed");
	}
	// SSL_CTX resource will now be guarded by a custom deleter
	itsCTX = boost::shared_ptr<SSL_CTX>(tmp, SSL_CTX_free);

	SSL_CTX_set_default_passwd_cb(itsCTX.get(), PasswordCallback);
	SSL_CTX_set_default_passwd_cb_userdata(itsCTX.get(), this);

	// Certificate
	DBG << "Certificate file=" << itsCertFile << std::endl;
	if ( itsCertFile != "") {
		if (SSL_CTX_use_certificate_file(itsCTX.get(), itsCertFile.c_str(), SSL_FILETYPE_PEM)<= 0) {
			throw_SSL("SSL_CTX_use_certificate_file failed");
		}
	}

	// Key
	DBG << "Key file=" << itsKeyFile << std::endl;
	if (itsKeyFile != "") {
		/* set the private key from keyFile */
		if (SSL_CTX_use_PrivateKey_file(itsCTX.get(), itsKeyFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
			throw_SSL("Loading Private key failed");
		}
		/* verify private key */
		if (!SSL_CTX_check_private_key(itsCTX.get())) {
			throw_SSL("Private key does not match the public certificate");
		}
	}

	// Trust
	DBG << "Trust file=" << itsTrustFile << std::endl;
	if (itsTrustFile != "") {
		SSL_CTX_load_verify_locations(itsCTX.get(), itsTrustFile.c_str(), NULL);
		SSL_CTX_set_verify(itsCTX.get(), SSL_VERIFY_PEER, NULL);
		// XXX: certificates CA chain length is hardcoded to 1 - is this needed in latest OpenSSL?
		SSL_CTX_set_verify_depth(itsCTX.get(), 1);
	}
}

SSLContext::~SSLContext()
{
	DBG_DESTRUCTOR;
}

SSLContext::operator SSL_CTX* ()
{
	return itsCTX.get();
}

SSLContext_p SSLContext::PopulateContext(	securityMode method,
											std::string certFile,
											std::string keyFile,
											std::string trustFile,
											std::string password)
{
	SSLContext_p p(new SSLContext(method, certFile, keyFile, trustFile, password));
	return p;
}

int SSLContext::PasswordCallback(char *buf, int size, int rwflag, void *userdata)
{
	SSLContext* obj = reinterpret_cast<SSLContext*>(userdata);

	if (obj != NULL) {
		DBG_FUNC << "Requesting password from "<< obj << std::endl;
		strncpy(buf, obj->itsPassword.c_str(), size);
		buf[size - 1] = '\0';
		return (strlen(buf));

	} else {
		//TODO: throw here? quit? returning zero might lead us to infinite-loop?
		ERR_FUNC << "Could not retrieve password from userdata=" << userdata << std::endl;
		return 0;
	}
}

const SSL_METHOD* SSLContext::GetMethod(securityMode mode)
{
	switch (mode) {
		case serverTLSv1: {
			return TLSv1_server_method();
		} break;
#ifndef OPENSSL_NO_SSL2
		case serverSSLv2: {
			return SSLv2_server_method();
		} break;
#endif
		case serverSSLv3: {
			return SSLv3_server_method();
		} break;
		case serverSSLv23: {
			return SSLv23_server_method();
		} break;
		case clientTLSv1: {
			return TLSv1_client_method();
		} break;
#ifndef OPENSSL_NO_SSL2
		case clientSSLv2: {
			return SSLv2_client_method();
		} break;
#endif
		case clientSSLv3: {
			return SSLv3_client_method();
		} break;
		case clientSSLv23: {
			return SSLv23_client_method();
		} break;
		default:
			assert(0);
	}
}

void SSLContext::libsslInit()
{
	//TODO: make it thread-safe (use semaphores?)
	static bool libsslReady = false;
	if (!libsslReady) {
		ERR_load_BIO_strings();
		SSL_load_error_strings(); /* readable error messages */
		SSL_library_init(); /* initialize library */
		libsslReady = true;
	}
}

}

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
#include <Utilities/Logger.h>
#include <Security/SecurityClientSSL.h>
#include <Exception/Exception.h>
#include <Security/Certificate/Certificate.h>
#include <Security/Key/Key.h>
#include <Security/Trust/Trust.h>
#include <Utilities/SSLWrap.h>

static osock::Logger logger("SecurityClientSSL");

namespace osock
{
SecurityClientSSL::SecurityClientSSL(	Address& Address,
										std::string certFile,
										std::string keyFile,
										std::string trustFile,
										std::string password,
										securityMode method) :
	SecurityClient(Address),
	itsSecurityMode(method),
	itsCertificate(new Certificate()), itsKey(new Key()), itsTrust(new Trust())
{
	SetCertificate(certFile);
	SetKey(keyFile);
	SetTrust(trustFile);
	itsPassword = password;
	libsslInit();

	itsCTX = SSLWrap::SSL_CTX_new(GetMethod()); /* create new context from method */
	if (itsCTX == NULL) {
		throw_SSL("SSL_CTX_new failed");
	}

	SSLWrap::SSL_CTX_set_default_passwd_cb(itsCTX, passwordCallback);
	if (itsPassword.length() >= 4)
		SSLWrap::SSL_CTX_set_default_passwd_cb_userdata(itsCTX, this);

	itsCertificate->SetContext(itsCTX);
	itsKey->SetContext(itsCTX);
	itsTrust->SetContext(itsCTX);

	itsCertificate->Apply();
	itsKey->Apply();
	itsTrust->Apply();

	//create new SSL BIO, basing on a configured context
	BIO* bio = SSLWrap::BIO_new_ssl_connect(itsCTX);
	if (bio == NULL) {
		throw_SSL("BIO_new_ssl_connect failed");
	}

	//make sure SSL is here
	SSLWrap::BIO_get_ssl_(bio, & itsSSL);
	if (itsSSL == NULL) {
		throw_SSL("BIO_get_ssl failed");
	}

	/* With this option set, if the server suddenly wants a new handshake,
	 * OpenSSL handles it in the background. */
	SSLWrap::SSL_set_mode_(itsSSL, SSL_MODE_AUTO_RETRY);

	/*The hostname can be an IP address. The hostname can also include the port
	 * in the form hostname:port . It is also acceptable to use the form
	 * "hostname/any/other/path" or "hostname:port/any/other/path".*/
	SSLWrap::BIO_set_conn_hostname_(bio, itsSrverAddress.GetHostAndPort().c_str());

	DBG << "populated safe client BIO @host=" << itsSrverAddress.GetHostAndPort() << std::endl;
	SetBIO(bio);

	DBG_CONSTRUCTOR;
}

SecurityClientSSL::~SecurityClientSSL()
{
	DBG_DESTRUCTOR;
	delete itsCertificate;
	delete itsKey;
	delete itsTrust;
	SSLWrap::SSL_CTX_free(itsCTX);
}

SSL* SecurityClientSSL::GetSSL()
{
	SSL* ssl = NULL;
	SSLWrap::BIO_get_ssl_(GetBIO(), &ssl);
	return ssl;
}

bool SecurityClientSSL::IsServerVerified()
{
	assert(GetSSL() != NULL );
	return (SSL_get_verify_result( GetSSL() ) == X509_V_OK);
}

void SecurityClientSSL::SetCertificate(std::string certFile)
{
	itsCertificate->SetFile(certFile);
}

void SecurityClientSSL::SetKey(std::string keyFile)
{
	itsKey->SetFile(keyFile);
}

void SecurityClientSSL::SetTrust(std::string trustFile)
{
	itsTrust->SetFile(trustFile);
	// setting appropriate trust level is needed to detect broken certificates
	itsTrust->SetVerify(Trust::trustRequire);
}

const SSL_METHOD* SecurityClientSSL::GetMethod()
{
	switch (itsSecurityMode) {
		case securityTLSv1: {
			return TLSv1_client_method();
		} break;
		case securitySSLv3: {
			return SSLv3_client_method();
		} break;
		case securitySSLv23: {
			return SSLv23_client_method();
		} break;
		default:
			assert(0);
	}
}

int SecurityClientSSL::passwordCallback(char *buf, int size, int rwflag, void *userdata)
{
	SecurityClientSSL* obj = reinterpret_cast<SecurityClientSSL*>(userdata);

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
}

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

#include "SecurityClientSSL.h"
#include "../Exception/Exception.h"

#include "Certificate/Certificate.h"
#include "Key/Key.h"
#include "Trust/Trust.h"

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
	DBG_CONSTRUCTOR;
}

SecurityClientSSL::~SecurityClientSSL()
{
	DBG_DESTRUCTOR;
}

bool SecurityClientSSL::IsPeerVerified()
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

SSL_METHOD* SecurityClientSSL::GetMethod()
{
	switch (itsSecurityMode) {
		case securityTLSv1: {
			return TLSv1_client_method();
		} break;
		case securitySSLv2: {
			return SSLv2_client_method();
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

BIO* SecurityClientSSL::PopulateBIO()
{
	itsCTX = SSL_CTX_new(GetMethod()); /* create new context from method */
	if (itsCTX == NULL) {
		throw_SSL("SSL_CTX_new failed");
	}

	SSL_CTX_set_default_passwd_cb(itsCTX, passwordCallback);
	if (itsPassword.length() >= 4)
		SSL_CTX_set_default_passwd_cb_userdata(itsCTX, this);

	itsCertificate->SetContext(itsCTX);
	itsKey->SetContext(itsCTX);
	itsTrust->SetContext(itsCTX);

	itsCertificate->Apply();
	itsKey->Apply();
	itsTrust->Apply();

	//create new SSL BIO, basing on a configured context
	BIO* bio = BIO_new_ssl_connect(itsCTX);
	if (bio == NULL) {
		throw_SSL("BIO_new_ssl_connect failed");
	}

	//make sure SSL is here
	BIO_get_ssl(bio, & itsSSL);
	if (itsSSL == NULL) {
		throw_SSL("BIO_get_ssl failed");
	}

	/* With this option set, if the server suddenly wants a new handshake,
	 * OpenSSL handles it in the background. */
	SSL_set_mode(itsSSL, SSL_MODE_AUTO_RETRY);

	/*The hostname can be an IP address. The hostname can also include the port
	 * in the form hostname:port . It is also acceptable to use the form
	 * "hostname/any/other/path" or "hostname:port/any/other/path".*/
	BIO_set_conn_hostname(bio, itsSrverAddress.GetHostAndPort().data());

	DBG << "populated safe client BIO @host=" << itsSrverAddress.GetHostAndPort() << std::endl;
	return bio;
}

int SecurityClientSSL::passwordCallback(char *buf, int size, int rwflag, void *userdata)
{
	SecurityClientSSL* obj = reinterpret_cast<SecurityClientSSL*>(userdata);

	if (obj != NULL) {
		DBG_FUNC << "Requesting password from "<< obj << std::endl;
		strncpy(buf, obj->itsPassword.data(), size);
		buf[size - 1] = '\0';
		return (strlen(buf));

	} else {
		//TODO: throw here? quit? returning zero might lead us to infinite-loop?
		ERR_FUNC << "Could not retrieve password from userdata=" << userdata << std::endl;
		return 0;
	}
}

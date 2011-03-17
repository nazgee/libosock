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

#define DEBUG_WANTED

#include <defines.h>
#include <Security/SecurityServerSSL.h>
#include <Exception/Exception.h>
#include <Security/Certificate/Certificate.h>
#include <Security/Key/Key.h>
#include <Security/Trust/Trust.h>

namespace osock
{
SecurityServerSSL::SecurityServerSSL(int listenPort,
		std::string certFile,
		std::string keyFile,
		std::string trustFile,
		std::string password,
		securityMode method) :
	SecurityServer(listenPort),
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

SecurityServerSSL::~SecurityServerSSL()
{
	DBG_DESTRUCTOR;
	delete itsCertificate;
	delete itsKey;
	delete itsTrust;
	SSL_CTX_free(itsCTX);
}

void SecurityServerSSL::SetCertificate(std::string certFile)
{
	itsCertificate->SetFile(certFile);
}

void SecurityServerSSL::SetKey(std::string keyFile)
{
	itsKey->SetFile(keyFile);
}

void SecurityServerSSL::SetTrust(std::string trustFile)
{
	itsTrust->SetFile(trustFile);
	itsTrust->SetVerify(Trust::trustRequire);
}

BIO* SecurityServerSSL::PopulateBIO()
{
	itsCTX = SSL_CTX_new(GetMethod()); /* create new context from method */
	if (itsCTX == NULL) {
		throw_SSL("SSL_CTX_new failed");
	}

	//TODO move password related stuff to Certificate or Key class
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
	BIO* ssl_bio = BIO_new_ssl(itsCTX, 0);
	if(ssl_bio == NULL) {
		throw_SSL("BIO_new_ssl failed");
	}

	/* The port is represented as a string of the form "host:port", where
	 * "host" is the interface to use and "port" is the port. Either or both
	 * values can be "*" which is interpreted as meaning any interface or port
	 * respectively. "port" has the same syntax as the port specified in
	 * BIO_set_conn_port() for connect BIOs, that is it can be a numerical port
	 * string or a string to lookup using getservbyname() and a string table. */
	std::string port = to_string(itsListenPort);
	BIO* accept_bio = BIO_new_accept( const_cast<char *>(port.data()) );
	if(accept_bio == NULL) {
		throw_SSL("BIO_new_accept failed");
	}
	BIO_set_accept_bios(accept_bio, ssl_bio);

	DBG << "populated safe server BIO @port=" << itsListenPort << std::endl;
	return accept_bio;
}

bool SecurityServerSSL::DoHandshake(BIO* clientToShake)
{
	int ret;
	do {
		ret = BIO_do_handshake(clientToShake);
		if (ret <= 0) {
			if (!BIO_should_retry(clientToShake)) {
				//handshake failed, and there is no need to retry
				WRN << "handhaking client failed ultimately" << std::endl;
				return false;
			} else {
				//handshake failed, but there is a chance it will succeed on retry
				DBG << "failed, retrying as recommended by BIO_should_retry()" << std::endl;
			}
		}
	} while(ret <= 0);

	DBG << "handshaking client succeeded" << std::endl;
	return true;
}

SSL_METHOD* SecurityServerSSL::GetMethod()
{
	switch (itsSecurityMode) {
		case securityTLSv1: {
			return TLSv1_server_method();
		} break;
		case securitySSLv2: {
			return SSLv2_server_method();
		} break;
		case securitySSLv3: {
			return SSLv3_server_method();
		} break;
		case securitySSLv23: {
			return SSLv23_server_method();
		} break;
		default:
			assert(0);
	}
}

int SecurityServerSSL::passwordCallback(char *buf, int size, int rwflag, void *userdata)
{
	SecurityServerSSL* obj = reinterpret_cast<SecurityServerSSL*>(userdata);

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
}

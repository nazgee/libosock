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

#ifndef ServerSecuritySSL_H_
#define ServerSecuritySSL_H_

#include "SecurityServer.h"

class Key;
class Certificate;
class Trust;
class SecurityServerSSL: public SecurityServer
{
public:

	SecurityServerSSL(	int listenPort,
						std::string certFile = "",
						std::string keyFile = "",
						std::string trustFile = "",
						std::string password = "",
						securityMode method = securitySSLv2);
	virtual ~SecurityServerSSL();

	void SetCertificate(std::string certFile);
	void SetKey(std::string keyFile);
	void SetTrust(std::string trustFile);

protected:
	SSL_CTX* itsCTX;
	SSL* itsSSL;
	std::string itsPassword;	//TODO move password to Certificate/Key class
	Certificate* itsCertificate;
	Key* itsKey;
	Trust* itsTrust;

	virtual BIO* PopulateBIO();
	virtual bool DoHandshake(BIO* bioToShake);
	SSL_METHOD* GetMethod();
	static int passwordCallback(char *buf, int size, int rwflag, void *usrdata);
};

#endif /* ServerSecuritySSL_H_ */

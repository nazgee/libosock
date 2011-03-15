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

#ifndef TRUST_H_
#define TRUST_H_

#include <openssl/ssl.h>
#include <string>

class Trust
{
public:
	//typedef int (*verifyCallback)(int, X509_STORE_CTX *);
	typedef enum { 	trustIgnore = SSL_VERIFY_NONE,
					trustRequire = SSL_VERIFY_PEER } trustMode;

					Trust(std::string trustFile = "", trustMode trustMode = trustIgnore, SSL_CTX* ctx = NULL);
	virtual ~Trust();

	void SetVerify(trustMode trustMode);
	void SetFile(std::string trustFile);
	void SetContext(SSL_CTX* ctx);

	void Apply();

private:
	SSL_CTX* itsCTX;
	trustMode itsMode;
	std::string itsTrutsFile;
};

#endif /* TRUST_H_ */

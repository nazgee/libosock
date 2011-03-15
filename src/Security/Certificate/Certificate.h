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

#ifndef CERTIFICATE_H_
#define CERTIFICATE_H_

#include <openssl/ssl.h>
#include <string>

class Certificate
{
protected:
	SSL_CTX* itsCTX;
	std::string itsCertFile;
public:
	Certificate(std::string certFile = "", SSL_CTX* ctx = NULL );
	Certificate(X509* cert, SSL_CTX* ctx);
	virtual ~Certificate();

	void SetFile(std::string certFile);
	void SetContext(SSL_CTX* context);

	void Apply();
};

#endif /* CERTIFICATE_H_ */

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
#include <Security/Certificate/Certificate.h>
#include <Exception/Exception.h>

#include <assert.h>

namespace osock
{
Certificate::Certificate(std::string certFile, SSL_CTX* ctx) :
	itsCTX(ctx),
	itsCertFile(certFile)
{
	DBG_CONSTRUCTOR;
}

Certificate::Certificate(X509* cert, SSL_CTX* ctx) :
	itsCTX(ctx)
{
	assert(0); //TODO to be implemented
	DBG_CONSTRUCTOR;
}

Certificate::~Certificate()
{
	DBG_DESTRUCTOR;
}

void Certificate::SetFile(std::string certFile)
{
	itsCertFile = certFile;
}

void Certificate::SetContext(SSL_CTX* context)
{
	itsCTX = context;
}

void Certificate::Apply()
{
	DBG << itsCertFile << itsCTX << std::endl;
	if ((itsCTX == NULL) || (itsCertFile == "")) {
		WRN << "Bad settings- not attempting Apply()" << std::endl;
		return;
	}

	if (SSL_CTX_use_certificate_file(itsCTX, itsCertFile.c_str(), SSL_FILETYPE_PEM)<= 0) {
		throw_SSL("SSL_CTX_use_certificate_file failed");
	}
}
}

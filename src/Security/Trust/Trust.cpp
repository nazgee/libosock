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
#include <Security/Trust/Trust.h>
#include <Exception/Exception.h>
using namespace osock;

Trust::Trust(std::string trustFile, trustMode mode, SSL_CTX* ctx) :
	itsCTX(ctx),
	itsMode(mode),
	itsTrutsFile(trustFile)
{
	DBG_CONSTRUCTOR;
}

Trust::~Trust()
{
	DBG_DESTRUCTOR;
}

void Trust::SetVerify(trustMode mode)
{
	itsMode = mode;
}

void Trust::SetFile(std::string trustFile)
{
	itsTrutsFile = trustFile;
}

void Trust::SetContext(SSL_CTX* ctx)
{
	itsCTX = ctx;
}

void Trust::Apply()
{
	DBG << itsMode << " " << itsTrutsFile << " " << itsCTX << std::endl;
	if ((itsCTX == NULL) || (itsTrutsFile == "")) {
		WRN << "Bad settings- not attempting Apply()" << std::endl;
		return;
	}

	SSL_CTX_load_verify_locations(itsCTX, itsTrutsFile.data(), NULL);
	SSL_CTX_set_verify(itsCTX, itsMode, NULL);
	/* certificates CA chain is hardcoded to 1 */
	SSL_CTX_set_verify_depth(itsCTX,1);
}

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

#include <Security/Key/Key.h>
#include <Exception/Exception.h>

static osock::Logger logger("Key");

namespace osock
{
Key::Key(std::string keyFile, const SSL_CTX* ctx) :
	itsCTX(ctx),
	itsKeyFile(keyFile)
{
	DBG_CONSTRUCTOR;
}

Key::~Key()
{
	DBG_DESTRUCTOR;
}

void Key::SetFile(std::string keyFile)
{
	itsKeyFile = keyFile;
}

void Key::SetContext(const SSL_CTX* context)
{
	itsCTX = context;
}

void Key::Apply()
{
	DBG << itsKeyFile << itsCTX << std::endl;
	if ((itsCTX == NULL) || (itsKeyFile == "")) {
		WRN << "Bad settings- not attempting Apply()" << std::endl;
		return;
	}

	/* set the private key from keyFile */
	if (SSL_CTX_use_PrivateKey_file(const_cast<SSL_CTX*>(itsCTX), itsKeyFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
		throw_SSL("Loading Private key failed");
	}
	/* verify private key */
	if (!SSL_CTX_check_private_key(itsCTX)) {
		throw_SSL("Private key does not match the public certificate");
	}
}
}

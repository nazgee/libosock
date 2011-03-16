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

#ifndef KEY_H_
#define KEY_H_

#include <openssl/ssl.h>
#include <string>

namespace osock
{
class Key
{
private:
	SSL_CTX* itsCTX;
	std::string itsKeyFile;
public:
	//TODO three types of constructors should be made available: RSA*/array/file
	Key(std::string keyFile = "", SSL_CTX* ctx = NULL);
	virtual ~Key();

	void SetFile(std::string keyFile);
	void SetContext(SSL_CTX* conext);

	void Apply();
};
} //namespace osock

#endif /* KEY_H_ */

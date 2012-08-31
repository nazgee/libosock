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

#ifndef OSOCKClientSecurity_H_
#define OSOCKClientSecurity_H_

#include "Security.h"

namespace osock
{
class SecurityClient: public Security
{
public:

	SecurityClient(Address& serverAddress);
	virtual ~SecurityClient();

	//! Returns whether server was verified (i.e. has valid certificate)
	virtual bool IsServerVerified();

protected:
	Address itsSrverAddress;
};
} //namespace osock

#endif /* ClientSecurity_H_ */

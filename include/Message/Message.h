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

#ifndef MESSAGES_H_
#define MESSAGES_H_

namespace osock
{
class Message
{
public:
	virtual ~Message(void)
	{
	}

	virtual char* Unpack(int& Bytes) const=0;
	virtual bool Pack(char* data, int bytesNumber, int chunkNumber)=0;
};
} //namespace osock



#endif /* MESSAGES_H_ */

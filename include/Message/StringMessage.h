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

#ifndef STRINGMESSAGE_H_
#define STRINGMESSAGE_H_

#include "Message.h"

namespace osock
{
class StringMessage: public Message
{
public:
	StringMessage(unsigned short dataLen);
	StringMessage(const char *data, int dataLen = -1);
	virtual ~StringMessage(void);

	virtual char* Unpack(int& data) const;
	virtual bool Pack(char* data, int dataLen, int chunkNumber);

	char* GetData(void) const;
	int GetDataSize(void) const;
	void SetDataSize(int dataLen);
	int GetFreeData(void) const;

	StringMessage& operator =(const char* data);

private:
	//! Holds content of text-message
	char *itsData;
	//! represents number of chars that still can be used
	unsigned short itsFreeSize;
	//! Number of data-bytes in itsData (including NULL-terminator)
	unsigned short itsDataSize;

	//! Copies \a len bytes from \a str to this StringMessage
	void SetData(const char* data, int dataLen = -1);
	//! Appends \a len bytes from \a str to this StringMessage
	void ExpandData(const char* data, int dataLen = -1);
};
} //namespace osock

#endif /* STRINGMESSAGE_H_ */

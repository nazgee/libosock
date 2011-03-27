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
#include <defines.h>
#include <Message/StringMessage.h>

#include <string.h>

namespace osock
{
StringMessage::StringMessage(unsigned short dataLen) :
	std::string(dataLen, '\0')
{
	DBG_CONSTRUCTOR;
}

StringMessage::StringMessage(const char* data) :
	std::string(data)
{
	DBG_CONSTRUCTOR;
}

StringMessage::~StringMessage(void)
{
	DBG_DESTRUCTOR;
}

char *StringMessage::Unpack(int& dataSize) const
{
	dataSize = this->length()+1;
	char *data = new char[this->length()+1];
	strcpy(data, this->c_str());
	return data;
}

bool StringMessage::Pack(char *data, int dataLen, int chunkNumber)
{
	if (chunkNumber > 1)
		*this += data;
	else
		*this = data;

	return (data[dataLen - 1] == 0);
}

}

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
#include <Message/StringMessage.h>

#include <string.h>

namespace osock
{
StringMessage::StringMessage(unsigned short dataLen) :
	std::string(dataLen, 'X')
{
	this->clear();
	DBG_CONSTRUCTOR;
}

StringMessage::StringMessage(const char* data) :
	std::string(data)
{
	SetComplete(true);
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

data_chunk StringMessage::Pack(data_chunk& data)
{
	DBG << "Size of data to Pack() " << data.size() << "B, so far packed " << this->size() << "B" << std::endl;
	//count non-NULL characters
	unsigned int chars = strnlen(&data[0], data.size());
	DBG << "Found " << chars << " non-NULL characters" << std::endl;

	//store all non-NULL characters- NULL is automatically added to end of std::string
	this->append(&data[0], chars);

	if (chars < data.size()) {	//NULL-char was found in data- message complete
		SetComplete(true);
		DBG << "Completed string with " << this->size() << "B (" << chars << " non-NULL, 1 NULL-terminator)" << std::endl;
		return data_chunk(data.begin()+ chars + 1, data.end());
	} else { //NULL-char was NOT found in data YET
		DBG << "Continuing with " << this->length() << "B so far" << std::endl;
		return data_chunk();
	}
}

void StringMessage::Clear()
{
	DBG << "Clearing buffer" << std::endl;
	this->clear();
}

}

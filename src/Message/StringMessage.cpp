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

//#define DEBUG_WANTED

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
	DBG_CONSTRUCTOR;
}

StringMessage::~StringMessage(void)
{
	DBG_DESTRUCTOR;
}

data_chunk StringMessage::Remains() const
{
	return itsRemains;
}

data_chunk StringMessage::Unpack() const
{
	return data_chunk(this->c_str(), this->c_str() + this->length() + 1);
}

void StringMessage::Pack(data_chunk& data)
{
	// Count non-NULL characters
	unsigned int chars = strnlen(&data[0], data.size());

	// Store all non-NULL characters
	// NULL is automatically added to end of std::string- no need to save it
	this->append(&data[0], chars);

	if (chars < data.size()) {
		// NULL-terminator was found in data- message complete
		SetComplete(true);
		DBG << "Completed string with " << this->size()
			<< "B (" << chars << " new non-NULL + 1 NULL)" << std::endl;
		itsRemains.assign(data.begin()+ chars + 1, data.end());
		return;
	} else {
		// NULL-terminator was NOT found in data YET
		DBG << "Continuing, " << this->size()
			<< "B so far (" << chars << " new non-NULL)" << std::endl;
		return;
	}
}

void StringMessage::Clear()
{
	DBG << "Clearing buffer" << std::endl;
	this->clear();
	itsRemains.clear();
}

}

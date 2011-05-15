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
StringMessage::StringMessage(unsigned short dataLen, const std::string& terminator) :
	std::string(dataLen, 'X'),
	itsTerminator(terminator)
{
	this->clear();
	DBG_CONSTRUCTOR;
}

StringMessage::StringMessage(const std::string& data, const std::string& terminator) :
	std::string(data),
	itsTerminator(terminator)
{
	//TODO: throw, when data does not match given terminator
	DBG_CONSTRUCTOR;
}

StringMessage::StringMessage(unsigned short dataLen) :
	std::string(dataLen, 'X'),
	itsTerminator("\0", 1)
{
	this->clear();
	DBG_CONSTRUCTOR;
}

StringMessage::StringMessage(const std::string& data) :
	std::string(data),
	itsTerminator("\0", 1)
{
	DBG_CONSTRUCTOR;
}

StringMessage::~StringMessage(void)
{
	DBG_DESTRUCTOR;
}

data_chunk StringMessage::Unpack() const
{
	return data_chunk(this->data(), this->data() + this->size());
}

void StringMessage::doFeed(data_chunk& data)
{
	this->append(data.begin(), data.end());
	size_t found = this->find(itsTerminator);

	if (std::string::npos != found) {
		// Terminator was found in data
		size_t remains_pos = found + itsTerminator.size();
		itsRemains.assign(this->begin() + remains_pos, this->end());
		this->erase(this->begin() + remains_pos, this->end());

		setIsComplete(true);
		DBG << "Completed string with " << this->size()
			<< "B (" << found << " + " << itsTerminator.size() << ")" << std::endl;
	} else {
		// Terminator was not found in data
		DBG << "Continuing, " << this->size() << "B so far" << std::endl;
	}
}

void StringMessage::doClear()
{
	DBG << "Clearing buffer" << std::endl;
	this->clear();
}

}

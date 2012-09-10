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
#include <Message/StringMessage.h>
#include <Exception/Exception.h>

#include <string.h>

static osock::Logger logger("StringMessage");

namespace osock
{
StringMessage::StringMessage(unsigned short dataLen,
		const std::string& terminator, std::string name) :
		Message(name),
	itsTerminator(terminator)
{
	this->reserve(dataLen);
	this->append(itsTerminator);
	DBG_CONSTRUCTOR;
}

StringMessage::StringMessage(const std::string& data,
		const std::string& terminator, std::string name) :
		Message(name),
		std::string(data),
		itsTerminator(terminator)
{
	this->append(itsTerminator);
	DBG_CONSTRUCTOR;
}

StringMessage::StringMessage(const StringMessage& copy_from_me) :
		Message((Message&)copy_from_me),
		std::string(copy_from_me.begin(), copy_from_me.end()),
		itsTerminator(copy_from_me.getTerminator())
{
	DBG_CONSTRUCTOR;
}

StringMessage::~StringMessage(void)
{
	DBG_DESTRUCTOR;
}

std::string StringMessage::getTerminator() const
{
	return itsTerminator;
}

void StringMessage::setTerminator(std::string terminator)
{
	itsTerminator = terminator;
}

std::string StringMessage::getString() const
{
	if (!isDeserializingComplete())
		throw Exception("getValue() called on incomplete StringMessage!");

	return this->substr(0, this->length() - itsTerminator.length());
}

void StringMessage::setString(const std::string& new_body)
{
	this->assign(new_body);
	this->append(itsTerminator);
}

data_chunk StringMessage::doSerialize() const
{
	return data_chunk(this->data(), this->data() + this->size());
}

void StringMessage::doDeserializeChunk(const data_chunk& data)
{
	this->append(data.begin(), data.end());
	size_t found = this->find(itsTerminator);

	if (std::string::npos != found) {
		// Terminator was found in data
		size_t remains_pos = found + itsTerminator.size();
		data_chunk remains(this->begin() + remains_pos, this->end());
		this->erase(this->begin() + remains_pos, this->end());
		DeserializingComplete(remains);
		DBG << "Completed string with " << this->size()
			<< "B (" << found << " + " << itsTerminator.size() << ")" << std::endl;
	} else {
		// Terminator was not found in data
		DBG << "Continuing, " << this->size() << "B so far" << std::endl;
	}
}

void StringMessage::doDeserializingRestart()
{
	DBG << "Clearing StringMessage" << std::endl;
	this->clear();
}

StringMessage* StringMessage::doClone() const
{
	return new StringMessage(*this);
}

std::string StringMessage::doToString() const
{
	std::string r;
	r = "string[" + to_string(getString().size()) + "]=" + getString()
	+ "; terminator[" + to_string(getTerminator().size()) + "]=" + getTerminator();

	return r;
}

}

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
class StringMessage: public Message, public std::string
{
private:
	std::string itsTerminator;

public:
	StringMessage(unsigned short dataLen, const std::string& terminator);
	StringMessage(const std::string& data, const std::string& terminator);
	StringMessage(unsigned short dataLen);
	StringMessage(const std::string& data);
	virtual ~StringMessage(void);

	std::string getTerminator();
	std::string setTerminator(std::string terminator);
	std::string getValue();

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(data_chunk& data);
	virtual void doClear();
};
} //namespace osock

#endif /* STRINGMESSAGE_H_ */

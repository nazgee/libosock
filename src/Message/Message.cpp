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
#include <Message/Message.h>
namespace osock
{
Message::Message() :
	isComplete(false)
{
}

data_chunk Message::Unpack() const
{
	return doUnpack();
}

bool Message::Pack(data_chunk& data)
{
	if (getIsComplete())
		Clear();

	doFeed(data);

	if (isComplete) {
		std::string s = getStringInfo();
		if (s.length())
			NFO << "Packed; " << s << std::endl;
		else
			DBG << "Packed; " << s << std::endl;
	}
	return getIsComplete();
}

void Message::Clear()
{
	doClear();
	itsRemains.clear();
	setIsComplete(false);
}

bool Message::getIsComplete() const
{
	return isComplete;
}

data_chunk Message::getRemains() const
{
	return itsRemains;
}

void Message::setIsComplete(bool isComplete)
{
	this->isComplete = isComplete;
}

std::string Message::getStringInfo()
{
	return "";
}
}

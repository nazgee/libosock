/*
 Copyright 2011, Michal Stawinski <michal.stawinski@gmail.com>

 This file is part of libsockets- C++ wrapper over OpenSSL and raw
 sockets.

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
//#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Utils.h>
#include <Utilities/Logger.h>
#include <Message/Message.h>
#include <Exception/Exception.h>

#include <typeinfo>

static osock::Logger logger("Message");

namespace osock
{
const std::string Message::ATTRBODY = "class=msgbody";
const std::string Message::ATTRNAME = "class=msgname";
const std::string Message::ATTRINFO = "class=msginfo";
const std::string Message::ATTRDATA = "class=msgdata";
const std::string Message::ATTRTAIL = "class=msgtail";
const std::string Message::TAG = "div";

Message::Message(std::string name) :
	itsState(MSG_ALLOWED_PACK_AND_UNPACK),
	itsMessageName(name)
{
}

Message::~Message()
{
}

Message* Message::Clone() const
{
	return doClone();
}

data_chunk Message::Unpack() const
{
	if (!isAllowed(MSG_ALLOWED_UNPACK))
		throw Exception("Unpack() called on incomplete Message!");

	return doSerialize();
}

std::string Message::UnpackAsTag(std::string tag, std::string attr, std::string tail) const
{
	std::string value;
	value += getAsTag(getMessageName(), TAG, ATTRNAME);
	value += getAsTag(getStringInfo(), TAG, ATTRINFO);
	value += getAsTag(Utils::DataToString(Unpack()), TAG, ATTRDATA);
	value += getAsTag(tail,TAG, ATTRTAIL);

	return getAsTag(value, tag, attr);
}

bool Message::Pack(const data_chunk& data)
{
	if (!isAllowed(MSG_ALLOWED_PACK))
		throw Exception("Pack() called when not allowed!");

	if (data.size() == 0) {
		return false;
	}

	if (getIsComplete()) {
		RestartPacking();
	}

	doFeed(data);

	if (getIsComplete()) {
		NFO << "Packed!"
						<< "; REST" << Utils::DataToString(itsRemains)
						<< "; INFO: " << getStringInfo() << std::endl;
	}

	return getIsComplete();
}

void Message::RestartPacking()
{
	doRestartPacking();
	itsRemains.clear();
	clearAllowed(MSG_ALLOWED_UNPACK);
}

void Message::ClosePacking(const data_chunk& remains)
{
	itsRemains = remains;
	setAllowed(MSG_ALLOWED_UNPACK);
	setAllowed(MSG_ALLOWED_REMAINS);
}

void Message::ExtendPacking()
{
	if (isAllowed(MSG_ALLOWED_REMAINS)) {
		DBG << "Pack() allowed, but prepend new data with Remains!" << std::endl;
		clearAllowed(MSG_ALLOWED_PACK);
	}

	clearAllowed(MSG_ALLOWED_UNPACK);
}

const std::string& Message::getMessageName() const
{
	return itsMessageName;
}

std::string Message::getAsTag(std::string value, std::string tag, std::string attr)
{
	if (tag.length()) {
		return std::string("<" + tag + " " + attr + ">" + value + "</" + tag + ">" + http::NEWLINE );
	} else {
		return value;
	}
}

std::string Message::getStringInfo() const
{
	return "?";
}

bool Message::getIsComplete() const
{
	return isAllowed(MSG_ALLOWED_UNPACK);
}

const data_chunk& Message::getRemains()
{
	if (!isAllowed(MSG_ALLOWED_REMAINS)) {
		WRN << "Why are you trying to read Remains (again)?" << std::endl;
		itsRemains.clear();
	}
	else {
		DBG << "You can now Pack() again!" << std::endl;
		setAllowed(MSG_ALLOWED_PACK);
		clearAllowed(MSG_ALLOWED_REMAINS);
	}

	return itsRemains;
}

}

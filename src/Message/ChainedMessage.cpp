/*
 * ChainedMessage.cpp
 *
 *  Created on: May 16, 2011
 *      Author: nazgee
 */

#include <defines.h>
//#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Logger.h>
#include <Message/ChainedMessage.h>
#include <Exception/Exception.h>

static osock::Logger logger("ChainedMessage");

namespace osock
{

ChainedMessage::ChainedMessage(std::string name) :
	Message(name),
	itsCurrentLink(-1)
{
	DBG_CONSTRUCTOR;
}

ChainedMessage::~ChainedMessage()
{
	DBG_DESTRUCTOR;
}

void ChainedMessage::AddLink(Message* mgs2add)
{
	DBG << "Added new link, while chain's getIsComplete()=" << isDeserializingComplete() << std::endl;
	//XXX: can this be safely removed?
//	mgs2add->DeserializingRestart();
	itsLinks.push_back(mgs2add);
	SerializingExtend();
}

const Message& ChainedMessage::getLastLink() const
{
	if (itsLinks.size()) {
		return itsLinks.at(itsLinks.size() - 1);
	} else {
		throw Exception("There are no links, can't return last one!");
	}
}

const Message& ChainedMessage::getLink(size_t link) const
{
	return itsLinks.at(link);
}

const int ChainedMessage::getLinksNumber() const
{
	return itsLinks.size();
}

void ChainedMessage::DeleteAllLinks()
{
	itsLinks.clear();
}

void ChainedMessage::LinksClose()
{
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		if (!itsLinks.at(i).isDeserializingComplete()) {
			std::string name = itsLinks.at(i).getName();
			WRN << "link(" << to_string(i) << ")=" << name << " is not complete!" << std::endl;
			throw Exception("Chain has incomplete links, can not be closed!");
		}
	}
	data_chunk empty;
	DeserializingComplete(empty);
}

std::string ChainedMessage::doToTag(std::string tag, std::string attr, std::string tail) const
{
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		tail += itsLinks.at(i).ToTag(tag, attr);
	}

	return Message::doToTag(tag, attr, tail);
}

osock::data_chunk ChainedMessage::doSerialize() const
{
	osock::data_chunk ret;

	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		osock::data_chunk tmp(itsLinks.at(i).Serialize());
		ret.insert(ret.end(), tmp.begin(), tmp.end());
	}

	return ret;
}

void ChainedMessage::doDeserializeChunk(const osock::data_chunk& data)
{
	itsBuffer = data;

	if (itsCurrentLink < 0) {
		itsCurrentLink = 0;
		itsLinks.at(0).DeserializingRestart();
	}

	while (itsLinks.at(itsCurrentLink).DeserializeChunk(itsBuffer)) {
		itsBuffer = itsLinks.at(itsCurrentLink).getDeserializingRemains();

		DBG << "Link " << to_string(itsCurrentLink) << " is complete"
				<< std::endl;

		itsCurrentLink++;
		if (itsCurrentLink >= static_cast<int> (itsLinks.size())) {
			DeserializingComplete(itsBuffer);
			break;
		} else {
			// Make sure next link is ready to be packed
			itsLinks.at(itsCurrentLink).DeserializingRestart();
		}
	}
	itsBuffer.clear();
}

void ChainedMessage::doDeserializingRestart()
{
	itsCurrentLink = -1;
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		itsLinks.at(i).DeserializingRestart();
	}
	DBG << "Cleared chain links" << std::endl;
}

ChainedMessage* ChainedMessage::doClone() const
{
	return new ChainedMessage(*this);
}

std::string ChainedMessage::doToString() const
{
	std::string s;
	s = "links_n=" + to_string(itsLinks.size());
	return s;
}

}

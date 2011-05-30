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
	DBG << "Added new link, while chain's getIsComplete()=" << getIsComplete() << std::endl;
	//XXX: can this be safely removed?
//	mgs2add->RestartPacking();
	itsLinks.push_back(mgs2add);
	ExtendPacking();
}

const Message& ChainedMessage::getLastLink() const
{
	if (itsLinks.size()) {
		return itsLinks[itsLinks.size() - 1];
	} else {
		throw Exception("There are no links, can't return last one!");
	}
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
		if (!itsLinks.at(i).getIsComplete()) {
			std::string name = itsLinks.at(i).getMessageName();
			WRN << "link(" << to_string(i) << ")=" << name << " is not complete!" << std::endl;
			throw Exception("Chain has incomplete links, can not be closed!");
		}
	}
	data_chunk empty;
	ClosePacking(empty);
}

std::string ChainedMessage::UnpackAsTag(std::string tag, std::string attr, std::string tail)
{
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		tail += itsLinks.at(i).UnpackAsTag(tag, attr);
	}

	return Message::UnpackAsTag(tag, attr, tail);
}

osock::data_chunk ChainedMessage::doUnpack() const
{
	osock::data_chunk ret;

	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		osock::data_chunk tmp(itsLinks[i].Unpack());
		ret.insert(ret.end(), tmp.begin(), tmp.end());
	}

	return ret;
}

void ChainedMessage::doFeed(const osock::data_chunk& data)
{
	itsBuffer = data;

	if (itsCurrentLink < 0) {
		itsCurrentLink = 0;
		itsLinks.at(0).RestartPacking();
	}

	while (itsLinks.at(itsCurrentLink).Pack(itsBuffer)) {
		itsBuffer = itsLinks.at(itsCurrentLink).getRemains();

		DBG << "Link " << to_string(itsCurrentLink) << " is complete"
				<< std::endl;

		itsCurrentLink++;
		if (itsCurrentLink >= static_cast<int> (itsLinks.size())) {
			ClosePacking(itsBuffer);
			break;
		} else {
			// Make sure next link is ready to be packed
			itsLinks.at(itsCurrentLink).RestartPacking();
		}
	}
	itsBuffer.clear();
}

void ChainedMessage::doRestartPacking()
{
	itsCurrentLink = -1;
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		itsLinks.at(i).RestartPacking();
	}
	DBG << "Cleared chain links" << std::endl;
}

std::string ChainedMessage::getStringInfo()
{
	std::string s;
	s = "links_n=" + to_string(itsLinks.size());
	return s;
}

}

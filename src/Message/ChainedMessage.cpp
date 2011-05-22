/*
 * ChainedMessage.cpp
 *
 *  Created on: May 16, 2011
 *      Author: nazgee
 */

#include <defines.h>
#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Logger.h>
#include <Message/ChainedMessage.h>
#include <Exception/Exception.h>

namespace osock
{

ChainedMessage::ChainedMessage() :
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
	if (!mgs2add->getIsComplete()) {
		DBG << "Marking chain as incomplete, because of new, incomplete chain" << std::endl;
		setIsComplete(false);
	}
	itsLinks.push_back(mgs2add);
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
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	itsRemains.insert(itsRemains.end(), data.begin(), data.end());

	if (itsCurrentLink < 0) {
		itsCurrentLink = 0;
		itsLinks[0].Clear();
	}

	while (itsLinks.at(itsCurrentLink).Pack(itsRemains)) {
		itsRemains = itsLinks.at(itsCurrentLink).getRemains();

		DBG << "Link " << to_string(itsCurrentLink) << " is complete"
				<< std::endl;

		itsCurrentLink++;
		if (itsCurrentLink >= static_cast<int> (itsLinks.size())) {
			setIsComplete(true);
			break;
		} else {
			// Make sure next link is Cleared
			itsLinks.at(itsCurrentLink).Clear();
		}
	}

	// Just to be sure, that keep remains only when we are done
	if (!getIsComplete()) {
		itsRemains.clear();
	}
}

void ChainedMessage::doClear()
{
	itsCurrentLink = -1;
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		itsLinks[i].Clear();
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

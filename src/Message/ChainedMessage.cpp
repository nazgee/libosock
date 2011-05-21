/*
 * ChainedMessage.cpp
 *
 *  Created on: May 16, 2011
 *      Author: nazgee
 */

#include <defines.h>
#include <Utilities/Logger.h>
#include <Message/ChainedMessage.h>

namespace osock
{

ChainedMessage::ChainedMessage()
{
	DBG_CONSTRUCTOR;
}

ChainedMessage::~ChainedMessage()
{
	DBG_DESTRUCTOR;
}

void ChainedMessage::AddLink(Message* mgs2add)
{
	itsLinks.push_back(mgs2add);
}

osock::data_chunk ChainedMessage::doUnpack() const
{
	osock::data_chunk ret;

	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		osock::data_chunk tmp = itsLinks[i].Unpack();
		ret.insert(ret.end(), tmp.begin(), tmp.end());
	}

	return ret;
}

void ChainedMessage::doFeed(const osock::data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	// Concatenate new data to old remainings
	itsRemains.insert(itsRemains.end(), data.begin(), data.end());

	// Feed as much links as possible with new data
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		bool complete = itsLinks[i].getIsComplete();

		if (!complete) {
			complete = itsLinks[i].Pack(itsRemains);
			itsRemains = itsLinks[i].getRemains();
		}

		if (!complete) {
			break;
		}

		if ( (i + 1) == itsLinks.size()) {
			setIsComplete(true);
			itsRemains = itsLinks[i].getRemains();
		}
	}
}

void ChainedMessage::doClear()
{
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		itsLinks[i].Clear();
	}
	DBG << "Cleared chain links" << std::endl;
}

std::string ChainedMessage::getStringInfo()
{
	std::string s;
	s = "links complete=" + to_string(itsLinks.size());
	return s;
}

}

/*
 * ChainedMessage.cpp
 *
 *  Created on: May 16, 2011
 *      Author: nazgee
 */

#include <defines.h>
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

void ChainedMessage::doFeed(osock::data_chunk& data)
{
	// Concatenate new data to old remainings
	itsRemains.insert(itsRemains.end(), data.begin(), data.end());

	// Feed as much links as possible with new data
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		bool complete = false;

		if (!itsLinks[i].getIsComplete()) {
			complete = itsLinks[i].Pack(itsRemains);
			itsRemains = itsLinks[i].getRemains();
		}

		if (!complete) {
			break;
		} else {
			// Chain is complete, when the last link is complete
			setIsComplete((i + 1) == itsLinks.size());
		}
	}
}

void ChainedMessage::doClear()
{
	//DBG << "Clearing links" << std::endl;
	for (unsigned int i = 0; i < itsLinks.size(); ++i) {
		itsLinks[i].Clear();
	}
}

}
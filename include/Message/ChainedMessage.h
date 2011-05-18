/*
 * ChainedMessage.h
 *
 *  Created on: May 16, 2011
 *      Author: nazgee
 */

#ifndef CHAINEDMESSAGE_H_
#define CHAINEDMESSAGE_H_

#include "Message.h"
#include <boost/ptr_container/ptr_vector.hpp>


namespace osock
{

class ChainedMessage: public osock::Message
{
private:
	boost::ptr_vector<Message> itsLinks;

public:
	ChainedMessage();
	~ChainedMessage();
	void AddLink(Message* mgs2add);

protected:
	virtual osock::data_chunk doUnpack() const;
	virtual void doFeed(osock::data_chunk& data);
	void virtual doClear();
};

}

#endif /* CHAINEDMESSAGE_H_ */

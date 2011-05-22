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
	int itsCurrentLink;
public:
	ChainedMessage();
	~ChainedMessage();
	void AddLink(Message* mgs2add);
	const Message& getLastLink() const;
	const int getLinksNumber() const;
	void DeleteAllLinks();

protected:
	virtual osock::data_chunk doUnpack() const;
	virtual void doFeed(const osock::data_chunk& data);
	void virtual doClear();
	std::string getStringInfo();
};

}

#endif /* CHAINEDMESSAGE_H_ */

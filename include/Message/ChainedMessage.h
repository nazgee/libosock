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
	ChainedMessage(std::string name = "ChainedMessage");
	~ChainedMessage();
	void AddLink(Message* mgs2add);
	const Message& getLastLink() const;
	const Message& getLink(size_t link) const;
	const int getLinksNumber() const;
	void DeleteAllLinks();
	void LinksClose();
	virtual std::string UnpackAsTag(std::string tag = Message::TAG, std::string attr = Message::ATTRBODY, std::string tail = "");

protected:
	virtual osock::data_chunk doUnpack() const;
	virtual void doFeed(const osock::data_chunk& data);
	virtual void doRestartPacking();
	virtual ChainedMessage* doClone() const;
	std::string getStringInfo() const;
};

}

#endif /* CHAINEDMESSAGE_H_ */

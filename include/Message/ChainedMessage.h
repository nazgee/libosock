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

protected:
	virtual osock::data_chunk doSerialize() const;
	virtual void doDeserializeChunk(const osock::data_chunk& data);
	virtual void doDeserializingRestart();
	virtual ChainedMessage* doClone() const;
    virtual std::string doToString() const;
	virtual std::string doToTag(std::string tag, std::string attr, std::string tail = "") const;
};

}

#endif /* CHAINEDMESSAGE_H_ */

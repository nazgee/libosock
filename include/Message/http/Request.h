/*
 * RequestLine.h
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#ifndef OSOCKREQUEST_H_
#define OSOCKREQUEST_H_

#include "../../Message/Message.h"
#include "../../Message/ChainedMessage.h"
#include "../../Message/http/Command.h"
#include "../../Message/http/Path.h"
#include "../../Message/http/Protocole.h"

namespace osock
{
namespace http
{

class Request: public Message
{
	enum LINKS {
			LINK_COMMAND,
			LINK_PATH,
			LINK_PROTOCOLE
	};
	ChainedMessage itsChain;

public:
	Request(std::string request = "GET", std::string path = "/",
			std::string protocole = "HTTP/1.0", std::string name = "Request");
	virtual ~Request();

	const Command& getCommand() const;
	const Path& getPath() const;
	const Protocole& getProtocole() const;


protected:
	virtual data_chunk doSerialize() const;
	virtual void doDeserializeChunk(const data_chunk& data);
	virtual void doDeserializingRestart();
	virtual Request* doClone() const;
	virtual std::string doToString() const;
	virtual std::string doToTag(std::string tag, std::string attr, std::string tail = "") const;

};

}	//namespace http
}	//namespace osock

#endif /* REQUEST_H_ */

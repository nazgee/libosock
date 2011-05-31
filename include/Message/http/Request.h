/*
 * RequestLine.h
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include "Message/Message.h"
#include "Message/ChainedMessage.h"
#include "Message/http/Command.h"
#include "Message/http/Path.h"
#include "Message/http/Protocole.h"

namespace osock
{
namespace http
{

class Request: public Message
{
	Command* itsCommand;
	Path* itsPath;
	Protocole* itsProtocole;
	ChainedMessage itsChain;
public:
	Request(std::string request = "GET", std::string path = "/",
			std::string protocole = "HTTP/1.0", std::string name = "Request");
	virtual ~Request();

	const Command& getCommand();
	const Path& getPath();
	const Protocole& getProtocole();

	virtual std::string UnpackAsTag(std::string tag = Message::TAG, std::string attr = Message::ATTRBODY, std::string tail = "");

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(const data_chunk& data);
	virtual void doRestartPacking();
	virtual std::string getStringInfo();

};

}	//namespace http
}	//namespace osock

#endif /* REQUEST_H_ */

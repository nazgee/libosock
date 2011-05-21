/*
 * RequestLine.h
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#ifndef REQUESTLINE_H_
#define REQUESTLINE_H_

#include "Message/Message.h"
#include "Message/ChainedMessage.h"
#include "Message/http/Request.h"
#include "Message/http/Path.h"
#include "Message/http/Protocole.h"

namespace osock
{

class RequestLine: public Message
{
	Request* itsRequest;
	Path* itsPath;
	Protocole* itsProtocole;
	ChainedMessage itsChain;
public:
	RequestLine(std::string request = "GET", std::string path = "/",
			std::string protocole = "HTTP/1.0");
	virtual ~RequestLine();

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(data_chunk& data);
	virtual void doClear();
	std::string getStringInfo();

};

}

#endif /* REQUESTLINE_H_ */

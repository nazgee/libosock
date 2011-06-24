/*
 * HttpRequest.h
 *
 *  Created on: May 21, 2011
 *      Author: nazgee
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_


#include <boost/ptr_container/ptr_vector.hpp>
#include <Message/Message.h>
#include <Message/ChainedMessage.h>
#include <Message/http/Request.h>

namespace osock
{

class HttpRequest: public osock::Message
{
private:
	http::Request itsRequest;
	ChainedMessage itsHeaders;

public:
	HttpRequest(std::string name = "HttpRequest");
	virtual ~HttpRequest();
	std::string getCommand();
	std::string getPath();
	std::string getProtocole();
	virtual std::string UnpackAsTag(std::string tag = Message::TAG, std::string attr = Message::ATTRBODY, std::string tail = "");

protected:
	virtual data_chunk doSerialize() const;
	virtual void doFeed(const data_chunk& data);
	void doFeedHeaders(const data_chunk& data);
	virtual void doRestartPacking();
	virtual HttpRequest* doClone() const;
	std::string getStringInfo() const;
};

}

#endif /* HTTPREQUEST_H_ */

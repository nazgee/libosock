/*
 * HttpResponse.h
 *
 *  Created on: May 27, 2011
 *      Author: nazgee
 */

#ifndef HTTPRESPONSE_H_
#define HTTPRESPONSE_H_

#include <Message/Message.h>
#include <Message/http/Response.h>
#include <Message/http/Header.h>
#include <Message/StringMessage.h>

namespace osock
{
namespace http
{
class Header;
}

class HttpResponse: public osock::Message
{
private:
	http::Response itsResponse;
	http::Header* itsHeaderDate;
	http::Header* itsHeaderType;
	http::Header* itsHeaderLength;
	ChainedMessage itsHeaders;
	StringMessage itsContent;

public:
	HttpResponse(std::string code = "200", std::string status = "OK",
			std::string content = "", std::string protocole = "HTTP/1.0");
	virtual ~HttpResponse();
	virtual std::string UnpackAsTag(std::string tag = Message::TAG, std::string attr = Message::ATTRBODY, std::string tail = "");

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(const data_chunk& data);
	virtual void doRestartPacking();
	virtual std::string getStringInfo();
private:
	void doFeedHeaders(const data_chunk& data);
	void doFeedContent(const data_chunk& data);
};
}

#endif /* HTTPRESPONSE_H_ */

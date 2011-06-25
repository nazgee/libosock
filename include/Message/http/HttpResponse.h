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
#include <Message/http/Status.h>
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
	enum LINKS {
		HEADER_DATE,
		HEADER_TYPE,
		HEADER_LENGTH
	};
	ChainedMessage itsHeaders;
protected:
	StringMessage itsContent;

public:
	HttpResponse(std::string content = "", std::string code = "200",
			std::string status = "OK", std::string protocole = "HTTP/1.0");
	HttpResponse(const http::Status& status, std::string content = "", std::string protocole = "HTTP/1.0");
	virtual ~HttpResponse();
	const StringMessage& getContent() const;
	const ChainedMessage& getHeaders() const;
	const http::Header& getHeaderDate() const;
	const http::Header& getHeaderType() const;
	const http::Header& getHeaderLength() const;
	const http::Response& getResponse() const;

protected:
	virtual data_chunk doSerialize() const;
	virtual void doDeserializeChunk(const data_chunk& data);
	virtual void doDeserializingRestart();
	virtual HttpResponse* doClone() const;
	virtual std::string doToString() const;
	virtual std::string doToTag(std::string tag, std::string attr, std::string tail = "") const;
private:
	void doFeedHeaders(const data_chunk& data);
	void doFeedContent(const data_chunk& data);
	void doInitHeaders();
};
}

#endif /* HTTPRESPONSE_H_ */

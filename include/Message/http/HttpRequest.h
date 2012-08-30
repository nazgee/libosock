/*
 * HttpRequest.h
 *
 *  Created on: May 21, 2011
 *      Author: nazgee
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_


#include <boost/ptr_container/ptr_vector.hpp>
#include "../../Message/Message.h"
#include "../../Message/ChainedMessage.h"
#include "../../Message/http/Request.h"

namespace osock
{

class HttpRequest: public osock::Message
{
private:
	http::Request itsRequest;
	ChainedMessage itsHeaders;

public:
	HttpRequest(std::string name = "HttpRequest");
	HttpRequest(http::Request& req, std::string name = "HttpRequest");
	virtual ~HttpRequest();
	std::string getCommand();
	std::string getPath();
	std::string getProtocole();

protected:
	virtual data_chunk doSerialize() const;
	virtual void doDeserializeChunk(const data_chunk& data);
	void doFeedHeaders(const data_chunk& data);
	virtual void doDeserializingRestart();
	virtual HttpRequest* doClone() const;
	virtual std::string doToString() const;
	virtual std::string doToTag(std::string tag, std::string attr, std::string tail = "") const;
};

}

#endif /* HTTPREQUEST_H_ */

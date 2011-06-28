/*
 * HttpRequest.cpp
 *
 *  Created on: May 21, 2011
 *      Author: nazgee
 */

#include <defines.h>
//#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Logger.h>
#include <Message/http/HttpRequest.h>
#include <Message/http/Header.h>

static osock::Logger logger("HttpRequest");

namespace osock
{

HttpRequest::HttpRequest(std::string name) :
	Message(name)
{
	DBG_CONSTRUCTOR;
}

HttpRequest::HttpRequest(http::Request& req, std::string name) :
	Message(name),
	itsRequest(req)
{
	char c[] = "\r\n";
	data_chunk d;

	d.insert(d.begin(), c, c + strlen(c));
	itsHeaders.AddLink(new http::Header());
	doFeedHeaders(d);

	DBG_CONSTRUCTOR;
}

HttpRequest::~HttpRequest()
{
	DBG_CONSTRUCTOR;
}

std::string HttpRequest::getCommand()
{
	return itsRequest.getCommand().getString();
}

std::string HttpRequest::getPath()
{
	return itsRequest.getPath().getString();
}

std::string HttpRequest::getProtocole()
{
	return itsRequest.getProtocole().getString();
}

std::string HttpRequest::doToTag(std::string tag, std::string attr, std::string tail) const
{
	return Message::doToTag(tag, attr, tail + itsRequest.ToTag(tag, attr)
		+ itsHeaders.ToTag(tag, attr));
}

data_chunk HttpRequest::doSerialize() const
{
	data_chunk ret(itsRequest.Serialize());
	data_chunk tmp(itsHeaders.Serialize());
	ret.insert(ret.end(), tmp.begin(), tmp.end());
	return ret;
}

void HttpRequest::doDeserializeChunk(const data_chunk& data)
{
	if (!itsRequest.isDeserializingComplete()) {
		if (itsRequest.DeserializeChunk(data)) {
			itsHeaders.AddLink(new http::Header());
			doFeedHeaders(itsRequest.getDeserializingRemains());
		}
	} else {
		doFeedHeaders(data);
	}
}

void HttpRequest::doFeedHeaders(const data_chunk& data)
{
	if (itsHeaders.DeserializeChunk(data)) {
		const http::Header& h =
				dynamic_cast<const http::Header&> (itsHeaders.getLastLink());

		if (h.IsHeadEmpty()) {
			DeserializingComplete(itsHeaders.getDeserializingRemains());
		} else {
			http::Header *hptr = new http::Header();
			itsHeaders.AddLink(hptr);
			DBG << "Got non empty header, waiting for another" << std::endl;
			doFeedHeaders(itsHeaders.getDeserializingRemains());
		}
	}
}

void HttpRequest::doDeserializingRestart()
{
	itsRequest.DeserializingRestart();
	itsHeaders.DeleteAllLinks();
	itsHeaders.DeserializingRestart();
}

HttpRequest* HttpRequest::doClone() const
{
	return new HttpRequest(*this);
}

std::string HttpRequest::doToString() const
{
	std::string s;
	s += "request=" + itsRequest.getCommand().getString() + " headers_n="
			+ to_string(itsHeaders.getLinksNumber());

	return s;
}

}

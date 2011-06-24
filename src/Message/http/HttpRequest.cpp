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

std::string HttpRequest::UnpackAsTag(std::string tag, std::string attr, std::string tail)
{
	return Message::UnpackAsTag(tag, attr, tail + itsRequest.UnpackAsTag(tag, attr)
		+ itsHeaders.UnpackAsTag(tag, attr));
}

data_chunk HttpRequest::doSerialize() const
{
	data_chunk ret(itsRequest.Unpack());
	data_chunk tmp(itsHeaders.Unpack());
	ret.insert(ret.end(), tmp.begin(), tmp.end());
	return ret;
}

void HttpRequest::doFeed(const data_chunk& data)
{
	if (!itsRequest.getIsComplete()) {
		if (itsRequest.Pack(data)) {
			itsHeaders.AddLink(new http::Header());
			doFeedHeaders(itsRequest.getRemains());
		}
	} else {
		doFeedHeaders(data);
	}
}

void HttpRequest::doFeedHeaders(const data_chunk& data)
{
	if (itsHeaders.Pack(data)) {
		const http::Header& h =
				dynamic_cast<const http::Header&> (itsHeaders.getLastLink());

		if (h.IsHeadEmpty()) {
			ClosePacking(itsHeaders.getRemains());
		} else {
			http::Header *hptr = new http::Header();
			itsHeaders.AddLink(hptr);
			DBG << "Got non empty header, waiting for another" << std::endl;
			doFeedHeaders(itsHeaders.getRemains());
		}
	}
}

void HttpRequest::doRestartPacking()
{
	itsRequest.RestartPacking();
	itsHeaders.DeleteAllLinks();
	itsHeaders.RestartPacking();
}

HttpRequest* HttpRequest::doClone() const
{
	return new HttpRequest(*this);
}

std::string HttpRequest::getStringInfo() const
{
	std::string s;
	s += "request=" + itsRequest.getCommand().getString() + " headers_n="
			+ to_string(itsHeaders.getLinksNumber());

	return s;
}

}

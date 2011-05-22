/*
 * HttpRequest.cpp
 *
 *  Created on: May 21, 2011
 *      Author: nazgee
 */

#include <defines.h>
#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Logger.h>
#include <Message/http/HttpRequest.h>
#include <Message/http/Header.h>

namespace osock
{

HttpRequest::HttpRequest()
{
	DBG_CONSTRUCTOR;
}

HttpRequest::~HttpRequest()
{
	DBG_CONSTRUCTOR;
}

data_chunk HttpRequest::doUnpack() const
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
			setIsComplete(true);
			itsRemains = itsHeaders.getRemains();
		} else {
			http::Header *hptr = new http::Header();
			hptr->Clear();
			itsHeaders.AddLink(hptr);
			DBG << "Got non empty header, waiting for another" << std::endl;
		}
	}
}

void HttpRequest::doClear()
{
	itsRequest.Clear();
	itsHeaders.DeleteAllLinks();
	itsHeaders.Clear();
}

std::string HttpRequest::getStringInfo()
{
	std::string s;
	s += "request=" + itsRequest.getCommand().getString() + " links_n="
			+ to_string(itsHeaders.getLinksNumber());

	return s;
}

}

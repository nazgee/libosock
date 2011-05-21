/*
 * HttpRequest.cpp
 *
 *  Created on: May 21, 2011
 *      Author: nazgee
 */

#include <Message/http/HttpRequest.h>

namespace osock
{

HttpRequest::HttpRequest()
{

}

HttpRequest::~HttpRequest()
{

}

data_chunk HttpRequest::doUnpack() const
{

}

void HttpRequest::doFeed(const data_chunk& data)
{
	if (!itsRequest.getIsComplete() && itsRequest.Pack(data)) {
		itsHeaders.Pack(itsRequest.getRemains());
	} else {
		setIsComplete(itsHeaders.Pack(data));
	}
}

void HttpRequest::doClear()
{

}

}

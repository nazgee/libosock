/*
 * Request.cpp
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#include <defines.h>
#include <Message/http/Request.h>

namespace osock
{

Request::Request(std::string request, std::string terminator) :
	StringMessage(request, terminator)
{
	DBG_CONSTRUCTOR;
}

Request::~Request()
{
	DBG_DESTRUCTOR;
}

data_chunk Request::doUnpack() const
{

}

void Request::doFeed(data_chunk& data)
{

}

void Request::doClear()
{

}

}

/*
 * Request.cpp
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
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

}

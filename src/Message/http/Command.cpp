/*
 * Request.cpp
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
#include <Message/http/Command.h>

namespace osock
{
namespace http
{

Command::Command(std::string command, std::string terminator) :
	StringMessage(command, terminator)
{
	DBG_CONSTRUCTOR;
}

Command::~Command()
{
	DBG_DESTRUCTOR;
}

}	//namespace http
}	//namespace osock

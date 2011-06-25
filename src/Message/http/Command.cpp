/*
 * Request.cpp
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
#include <Message/http/Command.h>

static osock::Logger logger("http::Command");

namespace osock
{
namespace http
{

Command::Command(std::string command, std::string terminator, std::string name) :
	StringMessage(command, terminator, name)
{
	DBG_CONSTRUCTOR;
}

Command::~Command()
{
	DBG_DESTRUCTOR;
}

std::string Command::doToString() const
{
	std::string s;
	s += "cmd=" + this->getString();
	return s;
}

Command* Command::doClone() const
{
	return new Command(*this);
}

}	//namespace http
}	//namespace osock

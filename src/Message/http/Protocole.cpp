/*
 * Protocole.cpp
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
#include <Message/http/Protocole.h>

static osock::Logger logger("http::Protocole");

namespace osock
{
namespace http
{

Protocole::Protocole(std::string protocole, std::string terminator, std::string name) :
		StringMessage(protocole, terminator, name)
{
	DBG_CONSTRUCTOR;
}

Protocole::~Protocole()
{
	DBG_DESTRUCTOR;
}

std::string Protocole::getStringInfo() const
{
	std::string s;
	s += "protocole=" + this->getString();
	return s;
}

Protocole* Protocole::doClone() const
{
	return new Protocole(*this);
}

}	//namespace http
}	//namespace osock

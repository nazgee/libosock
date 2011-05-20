/*
 * Protocole.cpp
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
#include <Message/http/Protocole.h>

namespace osock
{

Protocole::Protocole(std::string protocole, std::string terminator) :
		StringMessage(protocole, terminator)
{
	DBG_CONSTRUCTOR;
}

Protocole::~Protocole()
{
	DBG_DESTRUCTOR;
}

}

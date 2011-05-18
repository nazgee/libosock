/*
 * Protocole.cpp
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#include <defines.h>
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

data_chunk Protocole::doUnpack() const
{

}

void Protocole::doFeed(data_chunk& data)
{

}

void Protocole::doClear()
{

}

}

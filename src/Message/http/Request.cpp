/*
 * Request.cpp
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
#include "Message/http/Request.h"

namespace osock
{
namespace http
{

Request::Request(std::string request, std::string path,
		std::string protocole)
{
	itsCommand = new Command(request);
	itsChain.AddLink(itsCommand);

	itsPath = new Path(path);
	itsChain.AddLink(itsPath);

	itsProtocole = new Protocole(protocole);
	itsChain.AddLink(itsProtocole);

	DBG_CONSTRUCTOR;
}

Request::~Request()
{
	DBG_DESTRUCTOR;
}

data_chunk Request::doUnpack() const
{
	// it will throw, if called when not complete
	return itsChain.Unpack();
}

void Request::doFeed(const data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	setIsComplete(itsChain.Pack(data));
	itsRemains = itsChain.getRemains();
}

void Request::doClear()
{
	itsChain.Clear();
}

std::string Request::getStringInfo()
{
	std::string s;
	s = "cmd=" + itsCommand->getString()
		+ "; path=" + itsPath->getString()
		+ "; prot=" + itsProtocole->getString() + ";";
	return s;
}

}	//namespace http
}	//namespace osock

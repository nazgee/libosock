/*
 * RequestLine.cpp
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */
#define DEBUG_WANTED

#include <defines.h>
#include "Message/http/RequestLine.h"

namespace osock
{

RequestLine::RequestLine(std::string request, std::string path,
		std::string protocole)
{
	itsRequest = new Request(request);
	itsChain.AddLink(itsRequest);

	itsPath = new Path(path);
	itsChain.AddLink(itsPath);

	itsProtocole = new Protocole(protocole);
	itsChain.AddLink(itsProtocole);

	DBG_CONSTRUCTOR;
}

RequestLine::~RequestLine()
{
	DBG_DESTRUCTOR;
}

data_chunk RequestLine::doUnpack() const
{
	// it will throw, if called when not complete
	return itsChain.Unpack();
}

void RequestLine::doFeed(data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	setIsComplete(itsChain.Pack(data));

	if (getIsComplete())
		DBG << "requ=" << itsRequest->getValue()
			<< "; path=" << itsPath->getValue()
			<< "; prot=" << itsProtocole->getValue() << ";" << std::endl;
}

void RequestLine::doClear()
{
	itsChain.Clear();
}

}

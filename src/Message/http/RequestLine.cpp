/*
 * RequestLine.cpp
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#include <defines.h>
#include "Message/http/RequestLine.h"

namespace osock
{

RequestLine::RequestLine(std::string request, std::string path,
		std::string protocole)
{
	itsRequest = new Request("dupa");
	itsChain.AddLink(itsRequest);

	itsPath = new Path("dupa");
	itsChain.AddLink(itsPath);

	itsProtocole = new Protocole("dupa");
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
	DBG << "IsComplete = " << getIsComplete() << std::endl;
}

void RequestLine::doClear()
{
	itsChain.Clear();
}

}
/*
 * Request.cpp
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
#include "Message/http/Request.h"

static osock::Logger logger("http::Request");

namespace osock
{
namespace http
{

Request::Request(std::string request, std::string path,
		std::string protocole, std::string name) :
		Message(name)
{
	itsChain.AddLink(new Command(request));
	itsChain.AddLink(new Path(path));
	itsChain.AddLink(new Protocole(protocole));
	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Request::~Request()
{
	DBG_DESTRUCTOR;
}

const Command& Request::getCommand() const
{
	return dynamic_cast<const Command&>(itsChain.getLink(LINK_COMMAND));
}

const Path& Request::getPath() const
{
	return dynamic_cast<const Path&>(itsChain.getLink(LINK_PATH));
}

const Protocole& Request::getProtocole() const
{
	return dynamic_cast<const Protocole&>(itsChain.getLink(LINK_PROTOCOLE));
}

std::string Request::UnpackAsTag(std::string tag, std::string attr, std::string tail)
{
	return Message::UnpackAsTag(tag, attr,	tail + itsChain.UnpackAsTag(tag, attr));
}

data_chunk Request::doUnpack() const
{
	// it will throw, if called when not complete
	return itsChain.Unpack();
}

void Request::doFeed(const data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	if (itsChain.Pack(data)) {
		ClosePacking(itsChain.getRemains());
	}
}

void Request::doRestartPacking()
{
	itsChain.RestartPacking();
}

Request* Request::doClone() const
{
	return new Request(*this);
}

std::string Request::getStringInfo() const
{
	std::string s;
	s = "cmd=" + getCommand().getString()
		+ "; path=" + getPath().getString()
		+ "; prot=" + getProtocole().getString() + ";";
	return s;
}

}	//namespace http
}	//namespace osock

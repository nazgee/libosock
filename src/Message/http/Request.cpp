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

std::string Request::doToTag(std::string tag, std::string attr, std::string tail) const
{
	return Message::doToTag(tag, attr,	tail + itsChain.ToTag(tag, attr));
}

data_chunk Request::doSerialize() const
{
	// it will throw, if called when not complete
	return itsChain.Serialize();
}

void Request::doDeserializeChunk(const data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	if (itsChain.DeserializeChunk(data)) {
		DeserializingComplete(itsChain.getDeserializingRemains());
	}
}

void Request::doDeserializingRestart()
{
	itsChain.DeserializingRestart();
}

Request* Request::doClone() const
{
	return new Request(*this);
}

std::string Request::doToString() const
{
	std::string s;
	s = "cmd=" + getCommand().getString()
		+ "; path=" + getPath().getString()
		+ "; prot=" + getProtocole().getString() + ";";
	return s;
}

}	//namespace http
}	//namespace osock

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
		std::string protocole, std::string name) :
		Message(name)
{
	itsCommand = new Command(request);
	itsChain.AddLink(itsCommand);

	itsPath = new Path(path);
	itsChain.AddLink(itsPath);

	itsProtocole = new Protocole(protocole);
	itsChain.AddLink(itsProtocole);

	DBG_CONSTRUCTOR;
}

Request::Request(const Request& copy_from_me) :
	Message(copy_from_me)
{
	itsCommand = dynamic_cast<Command*>(copy_from_me.getCommand().Clone());
	itsChain.AddLink(itsCommand);

	itsPath = dynamic_cast<Path*>(copy_from_me.getPath().Clone());
	itsChain.AddLink(itsPath);

	itsProtocole = dynamic_cast<Protocole*>(copy_from_me.getProtocole().Clone());
	itsChain.AddLink(itsProtocole);

	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Request::~Request()
{
	DBG_DESTRUCTOR;
}

const Command& Request::getCommand() const
{
	return *itsCommand;
}

const Path& Request::getPath() const
{
	return *itsPath;
}

const Protocole& Request::getProtocole() const
{
	return *itsProtocole;
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
	s = "cmd=" + itsCommand->getString()
		+ "; path=" + itsPath->getString()
		+ "; prot=" + itsProtocole->getString() + ";";
	return s;
}

}	//namespace http
}	//namespace osock

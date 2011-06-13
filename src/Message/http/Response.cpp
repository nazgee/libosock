/*
 * Response.cpp
 *
 *  Created on: May 30, 2011
 *      Author: nazgee
 */

#include <defines.h>
#include <Message/http/Response.h>
//#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Logger.h>
namespace osock
{
namespace http
{
Response::Response(std::string protocole, std::string code,
		std::string status, std::string name) :
		Message(name)
{
	itsProtocole = new Protocole(protocole, " ");
	itsChain.AddLink(itsProtocole);

	itsStatus = new Status(code, status);
	itsChain.AddLink(itsStatus);

	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Response::Response(const Response& copy_from_me) :
	Message(copy_from_me)
{
	itsProtocole = dynamic_cast<Protocole*>(copy_from_me.getProtocole().Clone());
	itsChain.AddLink(itsProtocole);

	itsStatus = dynamic_cast<Status*>(copy_from_me.getStatus().Clone());
	itsChain.AddLink(itsStatus);

	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Response::~Response()
{
	DBG_DESTRUCTOR;
}

const Protocole& Response::getProtocole() const
{
	return *itsProtocole;
}

const Status& Response::getStatus() const
{
	return *itsStatus;
}

std::string Response::UnpackAsTag(std::string tag, std::string attr,
		std::string tail)
{
	return Message::UnpackAsTag(tag, attr,
			tail + itsChain.UnpackAsTag(tag, attr));
}

data_chunk Response::doUnpack() const
{
	// it will throw, if called when not complete
	return itsChain.Unpack();
}

void Response::doFeed(const data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	if (itsChain.Pack(data)) {
		ClosePacking(itsChain.getRemains());
	}
}

void Response::doRestartPacking()
{
	itsChain.RestartPacking();
}

Response* Response::doClone() const
{
	return new Response(*this);
}

std::string Response::getStringInfo() const
{
	std::string s;
	s = "protocole=" + itsProtocole->getString()
	+ "; status=" + itsStatus->getCode() + ", " + itsStatus->getStatus() + ";";
	return s;
}

} // namespace osock
} // namespace http

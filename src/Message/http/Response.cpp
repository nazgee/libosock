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

static osock::Logger logger("http::Response");

namespace osock
{
namespace http
{
Response::Response(std::string code, std::string status, std::string protocole, std::string name) :
		Message(name)
{
	itsChain.AddLink(new Protocole(protocole, " "));
	itsChain.AddLink(new Status(code, status));
	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Response::Response(const http::Status& status, std::string protocole, std::string name) :
		Message(name)
{
	itsChain.AddLink(new Protocole(protocole, " "));
	itsChain.AddLink(new Status(status));
	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Response::~Response()
{
	DBG_DESTRUCTOR;
}

const Status& Response::getStatus() const
{
	return dynamic_cast<const Status&>(itsChain.getLink(LINK_STATUS));
}

const Protocole& Response::getProtocole() const
{
	return dynamic_cast<const Protocole&>(itsChain.getLink(LINK_PROTOCOLE));
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
	s = "protocole=" + getProtocole().getString()
	+ "; status=" + getStatus().getCode() + ", " + getStatus().getStatus() + ";";
	return s;
}

} // namespace osock
} // namespace http

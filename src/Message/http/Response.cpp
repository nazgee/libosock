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

std::string Response::doToTag(std::string tag, std::string attr,
		std::string tail) const
{
	return Message::doToTag(tag, attr,
			tail + itsChain.ToTag(tag, attr));
}

data_chunk Response::doSerialize() const
{
	// it will throw, if called when not complete
	return itsChain.Serialize();
}

void Response::doDeserializeChunk(const data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	if (itsChain.DeserializeChunk(data)) {
		DeserializingComplete(itsChain.getDeserializingRemains());
	}
}

void Response::doDeserializingRestart()
{
	itsChain.DeserializingRestart();
}

Response* Response::doClone() const
{
	return new Response(*this);
}

std::string Response::doToString() const
{
	std::string s;
	s = "protocole=" + getProtocole().getString()
	+ "; status=" + getStatus().getCode().getString()
	+ ", " + getStatus().getStatus().getString() + ";";
	return s;
}

} // namespace osock
} // namespace http

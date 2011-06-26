/*
 * Status.cpp
 *
 *  Created on: May 30, 2011
 *      Author: nazgee
 */

#include <Message/StringMessage.h>
#include <Message/http/Status.h>
#include <Utilities/Logger.h>

static osock::Logger logger("http::Status");

namespace osock
{
namespace http
{

Status::Status(std::string code, std::string status, std::string name) :
	Message(name)
{
	itsChain.AddLink(new StringMessage(code, " "));
	itsChain.AddLink(new StringMessage(status, NEWLINE));

	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Status::Status(enum statusCode code, std::string name) :
	Message(name)
{
	std::string status = getDescription(code);

	itsChain.AddLink(new StringMessage(to_string(code), " "));
	itsChain.AddLink(new StringMessage(status, NEWLINE));

	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Status::~Status()
{
	DBG_DESTRUCTOR;
}

std::string Status::doToTag(std::string tag, std::string attr,
		std::string tail) const
{
	return Message::doToTag(tag, attr,
			tail + itsChain.ToTag(tag, attr));
}

std::string Status::getDescription(enum statusCode code)
{
	std::string explanation;

	switch (code) {
	case OK:
		explanation = "OK";
		break;
	case NOT_FOUND:
		explanation = "Not Found";
		break;
	case MOVED_PERM:
		explanation = "Moved Permanently ";
		break;
	case MOVED_TEMP:
		explanation = "Moved Temporarily";
		break;
	case SERVER_ERROR:
		explanation = "Server Error";
		break;
	default:
		explanation = "Unknown Error";
		break;
	}

	return explanation;
}

const StringMessage& Status::getCode() const
{
	return dynamic_cast<const StringMessage&>(itsChain.getLink(LINK_CODE));
}

const StringMessage& Status::getStatus() const
{
	return dynamic_cast<const StringMessage&>(itsChain.getLink(LINK_STATUS));
}

data_chunk Status::doSerialize() const
{
	// it will throw, if called when not complete
	return itsChain.Serialize();
}

void Status::doDeserializeChunk(const data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	if (itsChain.DeserializeChunk(data)) {
		DeserializingComplete(itsChain.getDeserializingRemains());
	}
}

void Status::doDeserializingRestart()
{
	itsChain.DeserializingRestart();
}

Status* Status::doClone() const
{
	return new Status(*this);
}

std::string Status::doToString() const
{
	std::string s;
	s = "code=" + getCode().getString() + "; status=" + getStatus().getString()
			+ ";";
	return s;
}

} // namespace http
} // namespace osock

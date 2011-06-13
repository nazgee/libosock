/*
 * Status.cpp
 *
 *  Created on: May 30, 2011
 *      Author: nazgee
 */

#include <Message/StringMessage.h>
#include <Message/http/Status.h>
#include <Utilities/Logger.h>

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

std::string Status::UnpackAsTag(std::string tag, std::string attr,
		std::string tail)
{
	return Message::UnpackAsTag(tag, attr,
			tail + itsChain.UnpackAsTag(tag, attr));
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

data_chunk Status::doUnpack() const
{
	// it will throw, if called when not complete
	return itsChain.Unpack();
}

void Status::doFeed(const data_chunk& data)
{
	DBG << "Got " << data.size() << "B to pack" << std::endl;
	if (itsChain.Pack(data)) {
		ClosePacking(itsChain.getRemains());
	}
}

void Status::doRestartPacking()
{
	itsChain.RestartPacking();
}

Status* Status::doClone() const
{
	return new Status(*this);
}

std::string Status::getStringInfo() const
{
	std::string s;
	s = "code=" + getCode() + "; status=" + getStatus()
			+ ";";
	return s;
}

} // namespace http
} // namespace osock

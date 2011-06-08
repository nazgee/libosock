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
	itsCode = new StringMessage(code, " ");
	itsChain.AddLink(itsCode);

	itsStatus = new StringMessage(status, NEWLINE);
	itsChain.AddLink(itsStatus);

	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Status::Status(enum statusCode code, std::string name) :
	Message(name)
{
	itsCode = new StringMessage(to_string(code), " ");
	itsChain.AddLink(itsCode);

	std::string status = getDescription(code);
	itsStatus = new StringMessage(status, NEWLINE);
	itsChain.AddLink(itsStatus);

	itsChain.LinksClose();

	DBG_CONSTRUCTOR;
}

Status::~Status()
{
	DBG_DESTRUCTOR;
}

std::string Status::getCode() const
{
	return itsCode->getString();
}

std::string Status::getStatus() const
{
	return itsStatus->getString();
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
		explanation = "Not found";
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
		explanation = "Unknown error";
		break;
	}

	return explanation;
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

std::string Status::getStringInfo()
{
	std::string s;
	s = "code=" + itsCode->getString() + "; status=" + itsStatus->getString()
			+ ";";
	return s;
}

} // namespace http
} // namespace osock

/*
 * HttpResponse.cpp
 *
 *  Created on: May 27, 2011
 *      Author: nazgee
 */

#include <defines.h>
#include <Message/http/HttpResponse.h>
#include <Message/http/Header.h>
//#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Logger.h>
#include <Utilities/Utils.h>
#include <Exception/Exception.h>

#include <time.h>

static osock::Logger logger("HttpResponse");

namespace osock
{

HttpResponse::HttpResponse(std::string content, std::string code,
		std::string status, std::string protocole) :
	Message("HttpResponse"),
	itsResponse(code, status, protocole),
	itsContent(content, http::NEWLINE)
{
	doInitHeaders();
	DBG_CONSTRUCTOR;
}

HttpResponse::HttpResponse(const http::Status& status, std::string content,
		std::string protocole) :
	Message("HttpResponse"), itsResponse(status, protocole),
	itsContent(content, http::NEWLINE)
{
	doInitHeaders();
	DBG_CONSTRUCTOR;
}

HttpResponse::~HttpResponse()
{
	DBG_DESTRUCTOR;
}

std::string HttpResponse::doToTag(std::string tag, std::string attr,
		std::string tail) const
{
	return Message::doToTag(
			tag,
			attr,
			tail + itsResponse.ToTag(tag, attr) + itsHeaders.ToTag(
					tag, attr) + itsContent.ToTag(tag, attr));
}

const StringMessage& HttpResponse::getContent() const
{
	return itsContent;
}

const ChainedMessage& HttpResponse::getHeaders() const
{
	return itsHeaders;
}

const http::Header& HttpResponse::getHeaderDate() const
{
	return dynamic_cast<const http::Header&>(itsHeaders.getLink(HEADER_DATE));
}

const http::Header& HttpResponse::getHeaderType() const
{
	return dynamic_cast<const http::Header&>(itsHeaders.getLink(HEADER_TYPE));
}

const http::Header& HttpResponse::getHeaderLength() const
{
	return dynamic_cast<const http::Header&>(itsHeaders.getLink(HEADER_LENGTH));
}

const http::Response& HttpResponse::getResponse() const
{
	return itsResponse;
}

data_chunk HttpResponse::doSerialize() const
{
	// Reevaluate content length
	const_cast<http::Header&>(getHeaderLength()).setHeadValue(to_string(getContent().length()));

	DBG << "About to start serializing" << std::endl;
	data_chunk ret(itsResponse.Serialize());
	DBG << "Serialized itsResponse" << std::endl;
	data_chunk tmph(itsHeaders.Serialize());
	ret.insert(ret.end(), tmph.begin(), tmph.end());
	DBG << "Serialized itsHeaders" << std::endl;
	data_chunk tmpc(itsContent.Serialize());
	ret.insert(ret.end(), tmpc.begin(), tmpc.end());
	DBG << "Serialized itsContent" << std::endl;
	return ret;
}

void HttpResponse::doDeserializeChunk(const data_chunk& data)
{
	if (!itsResponse.isDeserializingComplete()) {
		if (itsResponse.DeserializeChunk(data)) {
			itsHeaders.AddLink(new http::Header());
			doFeedHeaders(itsResponse.getDeserializingRemains());
		}
	} else {
		doFeedHeaders(data);
	}
}

void HttpResponse::doFeedHeaders(const data_chunk& data)
{
	DBG << "data=" << Utils::DataToString(data) << std::endl;
	if (!itsHeaders.isDeserializingComplete() && itsHeaders.DeserializeChunk(data)) {
		const http::Header& h =
				dynamic_cast<const http::Header&> (itsHeaders.getLastLink());

		if (h.IsHeadEmpty()) {
			doFeedContent(data);
		} else {
			http::Header *hptr = new http::Header();
			itsHeaders.AddLink(hptr);
			DBG << "Got non empty header, waiting for another" << std::endl;
			doFeedHeaders(itsHeaders.getDeserializingRemains());
		}
	} else if (itsHeaders.isDeserializingComplete()) {
		doFeedContent(data);
	}
}

void HttpResponse::doFeedContent(const data_chunk& data)
{
	DBG << "data=" << Utils::DataToString(data) << std::endl;
	if (itsContent.isDeserializingComplete()) {
		DeserializingComplete(itsContent.getDeserializingRemains());
	} else {
		itsContent.DeserializeChunk(data);
	}
}

void HttpResponse::doInitHeaders()
{
	time_t t;
	struct tm* tmp;
	char datestr[200];

	t = time(NULL);
	tmp = localtime(&t);
	if (tmp == NULL) {
		throw StdException("localtime() failed", errno);
	}

	if (strftime(datestr, sizeof(datestr), "%a, %d %b %Y %T %Z", tmp) == 0) {
		throw StdException("strfrtime() failed", errno);
	}

	itsHeaders.DeserializingRestart();

	itsHeaders.AddLink(new http::Header("Date", datestr));
	itsHeaders.AddLink(new http::Header("Content-Type", "text/html"));
	itsHeaders.AddLink(
			new http::Header("Content-Length", to_string(itsContent.length())));
	itsHeaders.AddLink(new http::Header("", ""));
	itsHeaders.LinksClose();
}

void HttpResponse::doDeserializingRestart()
{
	itsResponse.DeserializingRestart();
	itsHeaders.DeleteAllLinks();
	itsHeaders.DeserializingRestart();
	itsContent.DeserializingRestart();
}

HttpResponse* HttpResponse::doClone() const
{
	return new HttpResponse(*this);
}

std::string HttpResponse::doToString() const
{
	std::string s;
	s += "response=" + itsResponse.getStatus().getStatus() + " headers_n="
			+ to_string(itsHeaders.getLinksNumber()) + " content="
			+ itsContent.getString();

	return s;
}

}

/*
 * HttpResponseFile.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: nazgee
 */

#include <Message/http/HttpResponseFile.h>
#include <Exception/Exception.h>

namespace osock
{

HttpResponseFile::HttpResponseFile(std::string file, std::string code,
		std::string status, std::string protocole) :
		HttpResponse("", code, status, protocole),
		itsFileName(file)
{
	InitContent(itsFileName);
	DBG_CONSTRUCTOR;
}

HttpResponseFile::HttpResponseFile(const HttpResponseFile& copy_from_me) :
	HttpResponse(copy_from_me),
	itsFileName(copy_from_me.itsFileName)
{
	InitContent(itsFileName);
	DBG_CONSTRUCTOR;
}

HttpResponseFile::~HttpResponseFile()
{
	DBG_DESTRUCTOR;
}

void HttpResponseFile::InitContent(std::string filename)
{
	itsIFile.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!itsIFile.good()) {
		throw HttpException("Bad file given", http::Status::NOT_FOUND);
	}

	// get length of file:
	itsIFile.seekg(0, std::ios::end);
	int length = itsIFile.tellg();
	itsIFile.seekg(0, std::ios::beg);
	itsContent.resize(length);

	std::copy(std::istreambuf_iterator<char>(itsIFile),
			std::istreambuf_iterator<char>(), itsContent.begin());
	DBG << "Content of the file=" << getContent().UnpackAsTag();
}

HttpResponseFile* HttpResponseFile::doClone() const
{
	return new HttpResponseFile(*this);
}

}

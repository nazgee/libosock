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
		std::string status, std::string protocole)
{
	itsIFile.open(file.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!itsIFile.good()) {
		throw Exception("Bad file given");
	}

	// get length of file:
	itsIFile.seekg(0, std::ios::end);
	int length = itsIFile.tellg();
	itsIFile.seekg(0, std::ios::beg);
	getContent().resize(length);

	std::copy(std::istreambuf_iterator<char>(itsIFile),
			std::istreambuf_iterator<char>(), getContent().begin());

	DBG_CONSTRUCTOR;
	DBG << "Content of the file=" << getContent().UnpackAsTag();
}

HttpResponseFile::~HttpResponseFile()
{
	// TODO Auto-generated destructor stub
}

}

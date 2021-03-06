/*
 * Utils.cpp
 *
 *  Created on: May 23, 2011
 *      Author: nazgee
 */

#include <defines.h>
#include <Utilities/Utils.h>

#include <algorithm>
#include <openssl/err.h>

namespace osock
{

Utils::Utils()
{
	// TODO Auto-generated constructor stub

}

Utils::~Utils()
{
	// TODO Auto-generated destructor stub
}

void Utils::ShowCRLF(char& c)
{
	if(c == '\r') c = '~'; else if(c=='\n') c='^';
}

std::string Utils::DataToString(const data_chunk& chunk)
{
	std::string ret = "[" + to_string(chunk.size()) + "]={" + std::string(
			chunk.data(), chunk.size()) + "}";
	std::for_each( ret.begin(), ret.end(), ShowCRLF );
	return ret;
}

std::string Utils::StringToTag(std::string value, std::string tag, std::string attr)
{
	if (tag.length()) {
		return std::string("<" + tag + " " + attr + ">" + value + "</" + tag + ">" + http::NEWLINE );
	} else {
		return value;
	}
}

std::string Utils::getLastErrorSSL()
{
	// XXX is it thread safe?
	std::string s(ERR_error_string(ERR_get_error(), 0));
	return s;
}

}

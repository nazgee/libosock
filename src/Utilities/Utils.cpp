/*
 * Utils.cpp
 *
 *  Created on: May 23, 2011
 *      Author: nazgee
 */

#include <defines.h>
#include <Utilities/Utils.h>

#include <algorithm>

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

}

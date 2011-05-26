/*
 * Path.cpp
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
#include <Message/http/Path.h>

namespace osock
{
namespace http
{

Path::Path(std::string path, std::string terminator, std::string name) :
	StringMessage(path, terminator, name)
{
	DBG_CONSTRUCTOR;
}

Path::~Path()
{
	DBG_DESTRUCTOR;
}

std::string Path::getStringInfo()
{
	std::string s;
	s += "path=" + this->getString();
	return s;
}

}	//namespace http
}	//namespace osock

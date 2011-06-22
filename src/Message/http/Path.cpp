/*
 * Path.cpp
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#include <Utilities/Logger.h>
#include <Message/http/Path.h>

static osock::Logger logger("http::Path");

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

std::string Path::getStringInfo() const
{
	std::string s;
	s += "path=" + this->getString();
	return s;
}

Path* Path::doClone() const
{
	return new Path(*this);
}

}	//namespace http
}	//namespace osock

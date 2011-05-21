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

Path::Path(std::string path, std::string terminator) :
	StringMessage(path, terminator)
{
	DBG_CONSTRUCTOR;
}

Path::~Path()
{
	DBG_DESTRUCTOR;
}

}	//namespace http
}	//namespace osock

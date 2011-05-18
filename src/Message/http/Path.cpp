/*
 * Path.cpp
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#include <defines.h>
#include <Message/http/Path.h>

namespace osock
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

data_chunk Path::doUnpack() const
{

}

void Path::doFeed(data_chunk& data)
{

}

void Path::doClear()
{

}

}

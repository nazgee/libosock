/*
 * Request.h
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include "Message/StringMessage.h"

namespace osock
{

class Request: public StringMessage
{
public:
	Request(std::string request, std::string terminator = " ");
	virtual ~Request();

protected:

};

}

#endif /* REQUEST_H_ */

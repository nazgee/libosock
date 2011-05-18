/*
 * Request.h
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include "include/Message/StringMessage.h"

namespace osock
{

class Request: public osock::StringMessage
{
public:
	Request(std::string request, std::string terminator = " ");
	virtual ~Request();

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(data_chunk& data);
	virtual void doClear();

};

}

#endif /* REQUEST_H_ */

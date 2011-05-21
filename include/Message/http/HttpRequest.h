/*
 * HttpRequest.h
 *
 *  Created on: May 21, 2011
 *      Author: nazgee
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <Message/Message.h>

namespace osock
{

class HttpRequest: public osock::Message
{
public:
	HttpRequest();
	virtual ~HttpRequest();

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(data_chunk& data);
	virtual void doClear();
};

}

#endif /* HTTPREQUEST_H_ */

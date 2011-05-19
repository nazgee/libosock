/*
 * Header.h
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#ifndef HEADER_H_
#define HEADER_H_

#include "Message/StringMessage.h"

namespace osock
{

namespace http
{

class Header: public osock::StringMessage
{
private:
	std::string itsName;
	std::string itsValue;
	static const std::string separator;

protected:
	virtual void doFeed(data_chunk& data);
	virtual data_chunk doUnpack() const;

public:
	Header(std::string name = "name", std::string value = "value");
	virtual ~Header();

    std::string getHeadName() const;
    std::string getHeadValue() const;
    void setHeadName(std::string name);
    void setHeadValue(std::string value);
};

}

}

#endif /* HEADER_H_ */

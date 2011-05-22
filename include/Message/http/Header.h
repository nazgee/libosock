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
	virtual void doFeed(const data_chunk& data);
	virtual data_chunk doUnpack() const;
	virtual std::string getStringInfo();

public:
	Header(std::string name = "", std::string value = "");
	virtual ~Header();

    std::string getHeadName() const;
    std::string getHeadValue() const;
    bool IsHeadEmpty() const;
    void setHeadName(std::string name);
    void setHeadValue(std::string value);
};

}	//namespace http
}	//namespace osock
#endif /* HEADER_H_ */

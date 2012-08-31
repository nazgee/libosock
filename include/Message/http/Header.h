/*
 * Header.h
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#ifndef OSOCKHEADER_H_
#define OSOCKHEADER_H_

#include "../../Message/StringMessage.h"

namespace osock
{
namespace http
{

class Header: public osock::StringMessage
{
private:
	std::string itsKey;
	std::string itsValue;
	static const std::string separator;

protected:
	virtual void doDeserializeChunk(const data_chunk& data);
	virtual data_chunk doSerialize() const;
	virtual std::string doToString() const;
	virtual Header* doClone() const;

public:
	Header(std::string key = "", std::string value = "", std::string name = "Header");
	virtual ~Header();

    std::string getHeadKey() const;
    std::string getHeadValue() const;
    bool IsHeadEmpty() const;
    void setHeadKey(std::string name);
    void setHeadValue(std::string value);
};

}	//namespace http
}	//namespace osock
#endif /* HEADER_H_ */

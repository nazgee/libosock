/*
 * Header.cpp
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */



#include <defines.h>
#include <Utilities/Logger.h>
#include <Message/http/Header.h>
#include <Exception/Exception.h>

namespace osock
{
namespace http
{

const std::string Header::separator = ": ";

Header::Header(std::string name, std::string value) :
	StringMessage(name + separator + value, NEWLINE),
	itsName(name),
	itsValue(value)
{
	setIsComplete(true);
	DBG_CONSTRUCTOR;
}

Header::~Header()
{
	DBG_DESTRUCTOR;
}

void Header::doFeed(const data_chunk& data)
{
	StringMessage::doFeed(data);
	if (getIsComplete()) {
		if (getString().length() == 0) {
			setHeadName("");
			setHeadValue("");
		} else {
			size_t separator_pos = this->find(separator);

			if (separator_pos == std::string::npos) {
				throw Exception("Header does not contain '" + separator + "'");
			}

			itsName = this->substr(0, separator_pos);
			size_t value_pos = itsName.length() + separator.length();
			itsValue = this->substr(value_pos, this->length() - value_pos);

			if ((itsName.length() == 0) || (itsValue.length() == 0)){
				throw Exception("Header's name or value not given");
			}
		}
	}
}

data_chunk Header::doUnpack() const
{
	if (!getIsComplete())
		throw Exception("doUnpack() called on incomplete StringMessage!");

	data_chunk d;
	d.insert(d.end(), itsName.begin(), itsName.end());
	d.insert(d.end(), separator.begin(), separator.end());
	d.insert(d.end(), itsValue.begin(), itsValue.end());
	d.insert(d.end(), itsTerminator.begin(), itsTerminator.end());
	return d;
}

std::string Header::getStringInfo()
{
	std::string s;
	s ="name=" + getHeadName() + "; value=" + getHeadValue() + ";";
	return s;
}

std::string Header::getHeadName() const
{
	if (!getIsComplete())
		throw Exception("getName() called on incomplete Header!");

    return itsName;
}

std::string Header::getHeadValue() const
{
	if (!getIsComplete())
		throw Exception("getValue() called on incomplete Header!");

    return itsValue;
}

void Header::setHeadName(std::string name)
{
    this->itsName = name;
}

void Header::setHeadValue(std::string value)
{
    this->itsValue = value;
}

}	//namespace http
}	//namespace osock




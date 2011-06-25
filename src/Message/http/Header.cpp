/*
 * Header.cpp
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */


#include <defines.h>
//#define LOGLEVEL LOGLEVEL_DBG
#include <Utilities/Logger.h>
#include <Message/http/Header.h>
#include <Exception/Exception.h>

static osock::Logger logger("http::Header");

namespace osock
{
namespace http
{

const std::string Header::separator = ": ";

Header::Header(std::string key, std::string value, std::string name) :
	StringMessage(key + separator + value, NEWLINE, name),
	itsKey(key),
	itsValue(value)
{
	DBG_CONSTRUCTOR;
}

Header::~Header()
{
	DBG_DESTRUCTOR;
}

void Header::doDeserializeChunk(const data_chunk& data)
{
	StringMessage::doDeserializeChunk(data);
	if (isDeserializingComplete()) {
		if (getString().length() == 0) {
			setHeadKey("");
			setHeadValue("");
			DBG << "Decoded empty Header" << std::endl;
		} else {
			DBG << "Decoding: " << StringMessage::getString() << std::endl;

			size_t separator_pos = this->find(separator);

			if (separator_pos == std::string::npos) {
				throw Exception("Header does not contain '" + separator + "'");
			}

			itsKey = this->substr(0, separator_pos);
			size_t value_pos = itsKey.length() + separator.length();
			itsValue = this->substr(value_pos, getString().length() - value_pos);

			if ((itsKey.length() == 0) || (itsValue.length() == 0)){
				throw Exception("Header's name or value not given");
			}
		}
	}
}

data_chunk Header::doSerialize() const
{
	data_chunk d;
	if (itsKey.length()) {
		d.insert(d.end(), itsKey.begin(), itsKey.end());
		d.insert(d.end(), separator.begin(), separator.end());
		d.insert(d.end(), itsValue.begin(), itsValue.end());
	}
	d.insert(d.end(), itsTerminator.begin(), itsTerminator.end());
	return d;
}

std::string Header::doToString() const
{
	std::string s;
	s ="key[" + to_string(itsKey.length()) + "]=" + itsKey +
		"; value[" + to_string(itsValue.length()) + "]=" + itsValue + ";";
	return s;
}

Header* Header::doClone() const
{
	return new Header(*this);
}

std::string Header::getHeadKey() const
{
	if (!isDeserializingComplete())
		throw Exception("getName() called on incomplete Header!");

    return itsKey;
}

std::string Header::getHeadValue() const
{
	if (!isDeserializingComplete())
		throw Exception("getValue() called on incomplete Header!");

    return itsValue;
}

bool Header::IsHeadEmpty() const
{
	return ((getHeadKey() == "" ) && (getHeadValue() == ""));
}

void Header::setHeadKey(std::string key)
{
    this->itsKey = key;
}

void Header::setHeadValue(std::string value)
{
    this->itsValue = value;
}

}	//namespace http
}	//namespace osock




/*
 * Utils.h
 *
 *  Created on: May 23, 2011
 *      Author: nazgee
 */

#ifndef OSOCKUTILS_H_
#define OSOCKUTILS_H_

#include <string>
#include <Message/Message.h>

namespace osock
{

class Utils
{
private:
	Utils();
	void static ShowCRLF(char& c);

public:
	static std::string DataToString(const data_chunk& chunk);
	/**
	 * @brief Returns xml/html represenatation of a message
	 *
	 * @return std::string being xml/html represenatation of a message
	 */
	static std::string StringToTag(std::string value, std::string tag, std::string attr);
	static std::string getLastErrorSSL();
	virtual ~Utils();
};

}

#endif /* UTILS_H_ */

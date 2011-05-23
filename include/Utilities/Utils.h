/*
 * Utils.h
 *
 *  Created on: May 23, 2011
 *      Author: nazgee
 */

#ifndef UTILS_H_
#define UTILS_H_

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
	virtual ~Utils();
};

}

#endif /* UTILS_H_ */

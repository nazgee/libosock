/*
 * Request.h
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include "Message/StringMessage.h"

namespace osock
{
namespace http
{

class Command: public StringMessage
{
public:
	Command(std::string command, std::string terminator = " ");
	virtual ~Command();

protected:

};

}	//namespace http
}	//namespace osock

#endif /* COMMAND_H_ */

/*
 * Request.h
 *
 *  Created on: May 18, 2011
 *      Author: nazgee
 */

#ifndef OSOCCOMMAND_H_
#define OSOCCOMMAND_H_

#include "../../Message/StringMessage.h"

namespace osock
{
namespace http
{

class Command: public StringMessage
{
public:
	Command(std::string command, std::string terminator = " ",
			std::string name = "Command");
	virtual ~Command();

protected:
	virtual std::string doToString() const;
	virtual Command* doClone() const;

};

}	//namespace http
}	//namespace osock

#endif /* COMMAND_H_ */

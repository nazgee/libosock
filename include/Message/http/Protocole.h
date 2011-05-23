/*
 * Protocole.h
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#ifndef PROTOCOLE_H_
#define PROTOCOLE_H_

#include "Message/StringMessage.h"

namespace osock
{
namespace http
{

class Protocole : public StringMessage
{
public:
	Protocole(std::string protocole, std::string terminator = "\r\n");
	virtual ~Protocole();
	virtual std::string getStringInfo();

};

}	//namespace http
}	//namespace osock

#endif /* PROTOCOLE_H_ */

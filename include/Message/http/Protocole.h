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

class Protocole : public StringMessage
{
public:
	Protocole(std::string protocole, std::string terminator = "\r\n");
	virtual ~Protocole();

};

}

#endif /* PROTOCOLE_H_ */

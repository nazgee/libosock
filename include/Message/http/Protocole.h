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
	Protocole(std::string protocole, std::string terminator = "\r\n", std::string name = "Protocole");
	virtual ~Protocole();

protected:
	virtual std::string getStringInfo() const;
	virtual Protocole* doClone() const;

};

}	//namespace http
}	//namespace osock

#endif /* PROTOCOLE_H_ */

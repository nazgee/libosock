/*
 * Protocole.h
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#ifndef PROTOCOLE_H_
#define PROTOCOLE_H_

#include "include/Message/StringMessage.h"

namespace osock
{

class Protocole : public osock::StringMessage
{
public:
	Protocole(std::string protocole, std::string terminator = "\r\n");
	virtual ~Protocole();

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(data_chunk& data);
	virtual void doClear();

};

}

#endif /* PROTOCOLE_H_ */

/*
 * Path.h
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#ifndef PATH_H_
#define PATH_H_

#include "include/Message/StringMessage.h"

namespace osock
{

class Path : public osock::StringMessage
{
public:
	Path(std::string path, std::string terminator = " ");
	virtual ~Path();

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(data_chunk& data);
	virtual void doClear();

};

}

#endif /* PATH_H_ */

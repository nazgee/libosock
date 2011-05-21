/*
 * Path.h
 *
 *  Created on: May 19, 2011
 *      Author: nazgee
 */

#ifndef PATH_H_
#define PATH_H_

#include "Message/StringMessage.h"

namespace osock
{
namespace http
{

class Path : public StringMessage
{
public:
	Path(std::string path, std::string terminator = " ");
	virtual ~Path();

};

}	//namespace http
}	//namespace osock

#endif /* PATH_H_ */

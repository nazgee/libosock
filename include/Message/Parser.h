/*
 * Parser.h
 *
 *  Created on: Jul 4, 2011
 *      Author: nazgee
 */

#ifndef OSOCKPARSER_H_
#define OSOCKPARSER_H_

#include <boost/utility.hpp>
#include "../Message/Message.h"
#include "../BIO/BIO.h"

namespace osock
{
class Parser : private boost::noncopyable
{
private:
	BIO_p itsBIO;
	data_chunk itsRemainsOfData;
public:
	Parser(BIO_p bio);
	void Send(Message& msg) const;
	void Receive(Message& msg);
};
}
#endif /* PARSER_H_ */

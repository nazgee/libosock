/*
 * Status.h
 *
 *  Created on: May 30, 2011
 *      Author: nazgee
 */

#ifndef STATUS_H_
#define STATUS_H_

#include <Message/Message.h>
#include <Message/ChainedMessage.h>

namespace osock
{
class StringMessage;
namespace http
{
class Status: public osock::Message
{
private:
	StringMessage* itsCode;
	StringMessage* itsStatus;
	ChainedMessage itsChain;
public:
	Status(std::string code = "200", std::string status = "OK",
			std::string name = "Status");
	virtual ~Status();
	std::string getCode() const;
	std::string getStatus() const;
	virtual std::string UnpackAsTag(std::string tag, std::string attr, std::string tail);

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(const data_chunk& data);
	virtual void doRestartPacking();
	virtual std::string getStringInfo();
};

} // namespace http
} // namespace osock

#endif /* STATUS_H_ */

/*
 * Status.h
 *
 *  Created on: May 30, 2011
 *      Author: nazgee
 */

#ifndef STATUS_H_
#define STATUS_H_

#include "../../Message/Message.h"
#include "../../Message/ChainedMessage.h"

namespace osock
{
class StringMessage;
namespace http
{
class Status: public osock::Message
{
private:
	enum LINKS {
			LINK_CODE,
			LINK_STATUS
	};
	ChainedMessage itsChain;

public:
	enum statusCode {
		NOT_FOUND = 404,
		MOVED_TEMP = 301,
		MOVED_PERM = 302,
		SERVER_ERROR = 500,
		OK = 200
	};

	Status(std::string code, std::string status, std::string name = "Status");
	Status(enum statusCode code = OK, std::string name = "Status");
	virtual ~Status();
	const StringMessage& getCode() const;
	const StringMessage& getStatus() const ;
	static std::string getDescription(enum statusCode code);

protected:
	virtual data_chunk doSerialize() const;
	virtual void doDeserializeChunk(const data_chunk& data);
	virtual void doDeserializingRestart();
	virtual Status* doClone() const;
	virtual std::string doToString() const;
	virtual std::string doToTag(std::string tag, std::string attr, std::string tail = "") const;
};

} // namespace http
} // namespace osock

#endif /* STATUS_H_ */

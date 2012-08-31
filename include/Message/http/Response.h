/*
 * Response.h
 *
 *  Created on: May 30, 2011
 *      Author: nazgee
 */

#ifndef OSOCKRESPONSE_H_
#define OSOCKRESPONSE_H_

#include "../../Message/Message.h"
#include "../../Message/ChainedMessage.h"
#include "../../Message/http/Protocole.h"
#include "../../Message/http/Status.h"

namespace osock
{
namespace http
{
class Response: public osock::Message
{
private:
	enum LINKS {
		LINK_PROTOCOLE,
		LINK_STATUS
	};
	ChainedMessage itsChain;

public:
	Response(std::string code = "200", std::string status = "OK",
			std::string protocole = "HTTP/1.0", std::string name = "Response");
	Response(const http::Status& status, std::string protocole = "HTTP/1.0", std::string name = "Response");
	virtual ~Response();
	const Protocole& getProtocole() const;
	const Status& getStatus() const;

protected:
	virtual data_chunk doSerialize() const;
	virtual void doDeserializeChunk(const data_chunk& data);
	virtual void doDeserializingRestart();
	virtual Response* doClone() const;
	virtual std::string doToString() const;
	virtual std::string doToTag(std::string tag, std::string attr, std::string tail = "") const;
};

} // namespace http
} // namespace osock

#endif /* RESPONSE_H_ */

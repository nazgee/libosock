/*
 * Response.h
 *
 *  Created on: May 30, 2011
 *      Author: nazgee
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <Message/Message.h>
#include <Message/ChainedMessage.h>
#include <Message/http/Protocole.h>
#include <Message/http/Status.h>

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
	Response(std::string protocole = "HTTP/1.0", std::string code = "200",
			std::string status = "OK", std::string name = "Response");
	virtual ~Response();
	const Protocole& getProtocole() const;
	const Status& getStatus() const;
	virtual std::string UnpackAsTag(std::string tag = Message::TAG, std::string attr = Message::ATTRBODY, std::string tail = "");

protected:
	virtual data_chunk doUnpack() const;
	virtual void doFeed(const data_chunk& data);
	virtual void doRestartPacking();
	virtual Response* doClone() const;
	virtual std::string getStringInfo() const;
};

} // namespace http
} // namespace osock

#endif /* RESPONSE_H_ */

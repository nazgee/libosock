/*
 Copyright 2011, Michal Stawinski <michal.stawinski@gmail.com>

 This file is part of libsockets- C++ wrapper over OpenSSL and raw sockets.

 libsockets is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 libsockets is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with libsockets.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <vector>
#include <string>

namespace osock
{

typedef std::vector<char> data_chunk;

class Message
{
private:
	enum msg_state {
		MSG_ALLOWED_PACK = 0x1,
		MSG_ALLOWED_UNPACK = 0x2,
		MSG_ALLOWED_REMAINS = 0x4,
		MSG_ALLOWED_PACK_AND_UNPACK = MSG_ALLOWED_PACK | MSG_ALLOWED_UNPACK
	};
	//! Set to true by default- all messages should be Unpackable from the start
	data_chunk itsRemains;
	msg_state itsState;
	std::string itsMessageName;

	bool isAllowed(msg_state flag) const { return ((itsState & flag) == flag); }
	void setAllowed(msg_state flag) { int s = itsState; s |= static_cast<int>(flag); itsState = static_cast<msg_state>(s);}
	void clearAllowed(msg_state flag) { int s = itsState; s &= ~static_cast<int>(flag); itsState = static_cast<msg_state>(s);}

protected:
	//! Can be used freely- it's sole purpose is Derived classes convenience
	data_chunk itsBuffer;

public:
	Message(std::string name = "Message");
	virtual ~Message(void);

	data_chunk Unpack() const;
	virtual std::string UnpackAsTag(std::string tag, std::string attr, std::string tail = "");
	bool Pack(const data_chunk& data);
	void RestartPacking();
	bool getIsComplete() const;
	const data_chunk& getRemains();

protected:
	void ClosePacking(const data_chunk& remains);
	void ExtendPacking();
	std::string getMessageName();
	static std::string getAsTag(std::string value, std::string tag = "", std::string attr = "");
	virtual std::string getStringInfo();
	/**
	 * @brief Called to retrieve data_chunk representing message
	 *
	 * This method is called from Unpack() method. It should return data_chunk
	 * representing the message. This will be the data chunk sent when
	 * Socket::Send() method will be used.
	 *
	 * @note This method is called only if message is complete (meaning
	 * that getIsComplete() returns true), which means it is in a known state.
	 *
	 * @return Serialized message
	 */
	virtual data_chunk doUnpack() const = 0;
	/**
	 * @brief Called to deserialize message
	 *
	 * This method is called from Pack() method. Message should use provided
	 * data for deserialization. Concecutive chunks of data will be delivered
	 * until message is not marked as complete.
	 * @p Provided @c data_chunk can contain part of, whole, or even multiple
	 * serialized messages. In order to make retrieval of all the messages
	 * possible some rules should be followed during deserialization.
	 *  When provided serialized data are incomplete, doUnpack() should store
	 * or deserialize current data chunk, and return- it will be called again
	 * once new data become available. When after consecutive doUnpack() data
	 * calls Message is deserialized, doUnpack() should mark it as complete, and
	 * save part of data_chunk that was not used during deserialization.
	 *
	 * @note In order to mark message as complete, called setIsMessage()
	 * with @c true as argument.
	 * @p In order to save part of data_chunk that was not used (is not a part
	 * of current Message), doUnpack should assign it to itsRemains.
	 *
	 * @arg data Contains data that should be used to deserialize message. It
	 * can be part of, complete, or even multiple messages.
	 */
	virtual void doFeed(const data_chunk& data) = 0;
	/**
	 * @brief Called when internal state of the message should be cleared
	 *
	 * doRestartPacking() should result in restart of deserialization process-
	 * doUnpack() should treat consecutive data_chunk as a start of a message().
	 */
	virtual void doRestartPacking() = 0;

};
} //namespace osock


#endif /* MESSAGES_H_ */

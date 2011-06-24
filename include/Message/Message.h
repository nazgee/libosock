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

	Message* Clone() const;
	data_chunk Unpack() const;
	virtual std::string UnpackAsTag(std::string tag = Message::TAG, std::string attr = Message::ATTRBODY, std::string tail = "") const;
	bool Pack(const data_chunk& data);
	void RestartPacking();
	bool getIsComplete() const;
	const data_chunk& getRemains();
	const std::string& getMessageName() const;

	static const std::string ATTRBODY;
	static const std::string ATTRNAME;
	static const std::string ATTRINFO;
	static const std::string ATTRDATA;
	static const std::string ATTRTAIL;
	static const std::string TAG;

protected:
	/**
	 * @brief Marks Message as completely deserialized
	 * @note Should be called only from doSerialize(), when message was able to
	 * eventually deserialize istelf using provided data chunks
	 * @param remains Remainin data, that was not used during de
	 */
	void ClosePacking(const data_chunk& remains);
	/**
	 * @brief Marks Message as not fully deserialized
	 */
	void ExtendPacking();
	/**
	 * @brief Returns xml/html represenatation of a message
	 *
	 * Uses getStringInfo() to generate contents of the returned std::string
	 * @return std::string being xml/html represenatation of a message
	 */
	static std::string getAsTag(std::string value, std::string tag = "", std::string attr = "");
	/**
	 * @brief Returns std::string represenatation of a message (used for debugging)
	 * @return std::string represenatation of a message
	 */
	virtual std::string getStringInfo() const;
	/**
	 * @brief Returns data_chunk representing serialized Message
	 *
	 * Data chunk returned by this message should represent whole Message, and
	 * allow immediate deserialization when given to doFeed(). This will be data
	 * chunk sent when Socket::Send() method will be used.
	 *
	 * @note This method will NOT be called if message is in a middle of
	 * deserialization (using doFeed())
	 *
	 * @return Serialized message
	 */
	virtual data_chunk doSerialize() const = 0;
	/**
	 * @brief Feeds Message with data to be used for deserialization
	 *
	 * This method is called from Pack() whenever new data_chunk arrives.
	 * Consecutive chunks of data will be delivered until message will not
	 * mark itself as deserialized. Moreover, all extra data that was not used
	 * during message deserialization should be returned. Marking message as
	 * serialized and returning unused data is done in a single call to ClosePacking().
	 *
	 * @param data Contains data (possibly partial) that should be used
	 * during deserialization of the Message.
	 * @note Depending on Message and PDU size,different number of data chunks
	 * might be needed to fully deserialize Message. Single data chunk can
	 * contain only a part of, whole, or multiple Messages.
	 */
	virtual void doFeed(const data_chunk& data) = 0;
	/**
	 * @brief Called when internal state of the message should be cleared
	 *
	 * After this message is called, internal serialization/deserialization
	 * logic of the message should be reset. As a result, doFeed() should treat
	 * next data chunk as beginning of the Message, and
	 * doSerialize() should allow immediate deserialization.
	 */
	virtual void doRestartPacking() = 0;
	/**
	 * @brief Returns clone of the Message
	 *
	 * It is used to allow containers cloning. Often default copy constructor
	 * can be called here, but someteimes it has to be overriden.
	 * @return Clone of this Message
	 */
	virtual Message* doClone() const = 0;
};

/**
 * @brief used by boost::pointer_container library. Allows cloning containers
 * of Messages.
 * @param a Object to be cloned
 * @return Clone of the given object
 */
inline Message* new_clone( const Message& a )
{
    return a.Clone();
}

} //namespace osock


#endif /* MESSAGES_H_ */

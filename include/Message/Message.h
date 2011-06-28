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


#include <defines.h>

namespace osock
{

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

	// TODO make it a single and nice looking class, serving flags
	bool isAllowed(msg_state flag) const { return ((itsState & flag) == flag); }
	void setAllowed(msg_state flag) { int s = itsState; s |= static_cast<int>(flag); itsState = static_cast<msg_state>(s);}
	void clearAllowed(msg_state flag) { int s = itsState; s &= ~static_cast<int>(flag); itsState = static_cast<msg_state>(s);}

protected:
	//! Can be used freely- it's sole purpose is Derived classes convenience
	data_chunk itsBuffer;

public:
	Message(std::string name = "Message");
	virtual ~Message(void);
	/**
	 * @brief Returns clone of the Message
	 *
	 * It is used to allow containers cloning.
	 * @return Clone of this Message
	 * @note Relies on doClone() to do it's job
	 */
	Message* Clone() const;
	/**
	 * @brief Converts Message into chunk of data that can be send
	 * @return Chunk of data containing deserialized message
	 * @note Relies on doSerialize() to do it's job
	 */
	data_chunk Serialize() const;
	/**
	 * @brief Used to feed Message with chunk of data to be used to deserialize
	 * message
	 * @param data Chunk of data to be used for deserialization
	 * @return @c true if message is fully deserialized and does not need
	 * additional data chunks, @c false otherwise (i.e. Message needs more data
	 * in order fo finish deserialization)
	 * @note Relies on doDeserializeChunk() to do it's job
	 */
	bool DeserializeChunk(const data_chunk& data);
	/**
	 * @brief Stops current deserialization and allows to start over
	 * @note Relies on doDeserializingRestart() to do it's job
	 */
	void DeserializingRestart();
	/**
	 * @brief Returns true if message is completely deserialized
	 * @return @c true if message is fully deserialized, @c false otherwise (i.e.
	 * deserialization is in progress)
	 */
	bool isDeserializingComplete() const;
	const data_chunk& getDeserializingRemains();
	const std::string& getName() const;
	// TODO add class Tag, that would makes A LOT things more sexy and clean
	std::string ToTag(std::string tag, std::string attr, std::string tail = "") const;

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
	void DeserializingComplete(const data_chunk& remains);
	/**
	 * @brief Marks Message as not fully deserialized
	 */
	void SerializingExtend();
	/**
	 * @brief Implement it to allow restarting of deserialization
	 *
	 * After this message is called, internal serialization/deserialization
	 * logic of the message should be reset. As a result, doDeserializeChunk()
	 * should consider next data chunk as beginning of the Message, and
	 * doSerialize() should allow immediate deserialization.
	 */
	virtual void doDeserializingRestart() = 0;
	/**
	 * @brief Implement it to return data_chunk representing serialized Message
	 *
	 * Data chunk returned by this message should represent whole Message, and
	 * allow immediate deserialization when given to doDeserializeChunk(). This will be data
	 * chunk sent when Socket::Send() method will be used.
	 *
	 * @note This method will NOT be called if message is in a middle of
	 * deserialization (using doDeserializeChunk())
	 *
	 * @return Serialized message
	 */
	virtual data_chunk doSerialize() const = 0;
	/**
	 * @brief Implement it to accept data to be used for deserialization
	 *
	 * This method is called from DeserializeChunk() whenever new data_chunk arrives.
	 * Consecutive chunks of data will be delivered until message will not
	 * mark itself as deserialized. Moreover, all extra data that was not used
	 * during message deserialization should be returned. Marking message as
	 * serialized and returning unused data is done in a single call to DeserializingComplete().
	 *
	 * @param data Contains data (possibly partial) that should be used
	 * during deserialization of the Message.
	 * @note Depending on Message and PDU size,different number of data chunks
	 * might be needed to fully deserialize Message. Single data chunk can
	 * contain only a part of, whole, or multiple Messages.
	 */
	virtual void doDeserializeChunk(const data_chunk& data) = 0;
	/**
	 * @brief Implement it to returns clone of the Message
	 *
	 * Used to allow cloning containers that contain Messages. Usually default
	 * copy constructor can be used to create a new clone.
	 * @return Clone of this Message
	 */
	virtual Message* doClone() const = 0;
	/**
	 * @brief Implement it to return std::string represenatation of a message (used for debugging)
	 * @return std::string represenatation of a message
	 */
	virtual std::string doToString() const;
	/**
	 * @brief Implement it to return Tag represenatation of a message (used for debugging/presentation)
	 * @return Tag represenatation of a message
	 */
	virtual std::string doToTag(std::string tag, std::string attr, std::string tail = "") const;
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

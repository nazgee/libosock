/*
 * Parser.cpp
 *
 *  Created on: Jul 4, 2011
 *      Author: nazgee
 */
#include <Message/Parser.h>
#include <Utilities/Logger.h>
#include <Utilities/Utils.h>
static osock::Logger logger("Parser");

namespace osock
{

Parser::Parser(BIO_p bio) :
	itsBIO(bio)
{
	DBG_CONSTRUCTOR;
}

void Parser::Send(Message& msg) const
{
	itsBIO->WriteWithRetry(msg.Serialize());
}

void Parser::Receive(Message& msg)
{
	DBG << "Getting message ready for deserialiaztion; Remains"
			<< Utils::DataToString(itsRemainsOfData) << std::endl;
	msg.DeserializingRestart();

	if (!msg.DeserializeChunk(itsRemainsOfData)) {
		DBG << "Waiting for data" << std::endl;
		do {
			itsRemainsOfData = itsBIO->ReadWithRetry();
		} while (!msg.DeserializeChunk(itsRemainsOfData));
	}

	// Store excessive data for future use
	itsRemainsOfData = msg.getDeserializingRemains();
	DBG << "Message Received(); Remains from current read:"
			<< Utils::DataToString(itsRemainsOfData) << std::endl;
}

void Parser::ReceiveWithoutRetry(Message& msg)
{
	DBG << "Getting message ready for deserialiaztion; Remains"
			<< Utils::DataToString(itsRemainsOfData) << std::endl;
	msg.DeserializingRestart();

	if (!msg.DeserializeChunk(itsRemainsOfData)) {
		DBG << "Waiting for data" << std::endl;
		do {
			itsRemainsOfData = itsBIO->Read();
		} while (!msg.DeserializeChunk(itsRemainsOfData));
	}

	// Store excessive data for future use
	itsRemainsOfData = msg.getDeserializingRemains();
	DBG << "Message Received(); Remains from current read:"
			<< Utils::DataToString(itsRemainsOfData) << std::endl;
}

void Parser::Flush() {
	itsRemainsOfData.clear();
}

}

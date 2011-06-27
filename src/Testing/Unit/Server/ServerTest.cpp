/*
 * ServerTest.cpp
 *
 *  Created on: Jun 26, 2011
 *      Author: nazgee
 */
#include <osock.h>
#include <Testing/Unit/Server/ServerTest.h>
#include <Testing/testutils.h>

class ChallangeEcho : public Challange
{
public:
	bool Test(osock::SocketClient* client)
	{
		osock::StringMessage m1("PINGY-PONGY", osock::http::NEWLINE);
		osock::StringMessage m2(20, osock::http::NEWLINE);
		client->Send(m1);
		client->Receive(m2);

		return (m1 == m2);
	}

};

class ServerTest_Echo : public ServerTest
{
	CPPUNIT_TEST_SUITE(ServerTest_Echo);
	SERVER_TESTS_PACK();
	CPPUNIT_TEST_SUITE_END();

	virtual osock::ServerEcho* setUpServer(osock::SocketServer* socketServer)
	{
		return new osock::ServerEcho(socketServer);
	}
	virtual void setUpChallanges()
	{
		installChallange(new ChallangeEcho());
	}
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServerTest_Echo, Utils::getSuiteNameUnit());

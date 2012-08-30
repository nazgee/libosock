/*
 * StringMessageTest.cpp
 *
 *  Created on: Jun 15, 2011
 *      Author: nazgee
 */
#include <osock.h>
#include <testutils.h>
#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

class SingleClientEchoServer : public osock::ServerEcho {
public:
	SingleClientEchoServer(osock::Auth_p auth, std::string portname) :
		 osock::ServerEcho(auth, portname) {
	}

	virtual bool onServed(osock::Address& servedClient) {
		return false;
	}
};

class EchoServerTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(EchoServerTest);
  CPPUNIT_TEST(serverShouldWork);
  CPPUNIT_TEST_SUITE_END();


	osock::ServerEcho* echosrv;
	static std::string MESSAGE;

public:
	void setUp(void) {
		int port = Utils::getFreePort(10000,10000);
		echosrv = new SingleClientEchoServer(osock::AuthNone::Populate(), Utils::to_string(port));
	}
	void tearDown(void) {
		delete echosrv;
	}

protected:
	static void* pingerThread(void* port)
	{
		osock::StringMessage message (MESSAGE, osock::http::NEWLINE);
		sleep(1);
		std::string host("localhost:");
		host += Utils::to_string((intptr_t)port);

		osock::BIOSocket_p c = osock::BIOSocket::PopulateClientBIO(host);
		c->setWriteTimeout(100);
		osock::Parser parser(c);

		parser.Send(message);
		parser.Receive(message);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("echoed message shall be equal to sent message", message.getString(), MESSAGE);

		pthread_exit(NULL);
	}

	void serverShouldWork(void)
	{
		pthread_t pinger;
		intptr_t port = atoi(echosrv->getPortName().c_str());
		int res = pthread_create(&pinger, NULL, pingerThread, (void*) port);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("problems with pthread_create()", 0, res);

		echosrv->Start();
	}
};
std::string EchoServerTest::MESSAGE = "pingmessage";

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EchoServerTest, Utils::getSuiteServerEcho());
CPPUNIT_REGISTRY_ADD(Utils::getSuiteServerEcho(), Utils::getSuiteGroupServer());


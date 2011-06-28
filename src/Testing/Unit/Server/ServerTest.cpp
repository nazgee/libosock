///*
// * ServerTest.cpp
// *
// *  Created on: Jun 26, 2011
// *      Author: nazgee
// */
//#include <osock.h>
//#include <Testing/Unit/Server/ServerTest.h>
//#include <Testing/testutils.h>
//
//class ChallangeEcho: public Challange
//{
//public:
//	bool Test(osock::SocketClient* client)
//	{
//		osock::StringMessage m1("PINGY-PONGY", osock::http::NEWLINE);
//		osock::StringMessage m2(20, osock::http::NEWLINE);
//		client->Send(m1);
//		client->Receive(m2);
//
//		return (m1 == m2);
//	}
//};
//
//class ServerTest_Echo: public ServerTest
//{
//CPPUNIT_TEST_SUITE(ServerTest_Echo);
//		SERVER_TESTS_PACK();
//	CPPUNIT_TEST_SUITE_END();
//
//	virtual osock::ServerEcho* setUpServer(osock::SocketServer* socketServer)
//	{
//		return new osock::ServerEcho(socketServer);
//	}
//	virtual void setUpChallanges()
//	{
//		installChallange(new ChallangeEcho());
//	}
//};
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServerTest_Echo, Utils::getSuiteServerEcho());
//CPPUNIT_REGISTRY_ADD(Utils::getSuiteServerEcho(), Utils::getSuiteGroupServer());
//
//static osock::data_chunk pingmessage;
//class BIOServerTest : public CPPUNIT_NS::TestCase
//{
//	CPPUNIT_TEST_SUITE(BIOServerTest);
//	CPPUNIT_TEST(serverShouldWork);
//	CPPUNIT_TEST_SUITE_END();
//public:
//	void setUp(void)
//	{
//		std::string s("MyPingMessage\r\n");
//		pingmessage.clear();
//		pingmessage.insert(pingmessage.begin(), s.data(), s.data() + s.size());
//	}
//	void tearDown(void)
//	{
//	}
//
//protected:
//	static void* pingerThread(void* port)
//	{
//		sleep(1);
//		std::string host("localhost:");
//		host += Utils::to_string((unsigned int)port);
//
//		osock::BIOSocket_p c = osock::BIOSocket::PopulateClientBIO(host);
//		c->setWriteTimeout(100);
//		c->Write(pingmessage);
//
//		pthread_exit(NULL);
//	}
//
//	void serverShouldWork(void)
//	{
//		pthread_t pinger;
//		int port = Utils::getFreePort(10000,10000);
//
//		osock::ServerEcho s(osock::AuthNone::Populate(), Utils::to_string(port), osock::Server::serviceProcess);
//
//		int res = pthread_create(&pinger, NULL, pingerThread, (void*) port);
//		CPPUNIT_ASSERT_EQUAL_MESSAGE("problems with pthread_create()", 0, res);
//		//pthread_detach(pinger);
//
//		s.Start();
////		osock::BIOSocket_p c = s->AcceptIncoming();
////		osock::data_chunk d = c->Read();
////		CPPUNIT_ASSERT_MESSAGE("Data received from client shall be equal to sent", pingmessage == d);
//	}
//};
//
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BIOServerTest, Utils::getSuiteBIOServerTest());
//CPPUNIT_REGISTRY_ADD(Utils::getSuiteBIOServerTest(), Utils::getSuiteGroupServer());
//CPPUNIT_REGISTRY_ADD(Utils::getSuiteGroupServer(), Utils::getSuiteGroupUnit());

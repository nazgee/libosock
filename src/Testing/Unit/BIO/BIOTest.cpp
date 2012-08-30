/*
 * BIOTest.cpp
 *
 *  Created on: Jun 15, 2011
 *      Author: nazgee
 */
#include <osock.h>
#include <testutils.h>
#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

class BIOTest: public CPPUNIT_NS::TestCase
{
CPPUNIT_TEST_SUITE(BIOTest);
		CPPUNIT_TEST(constructorsShallNotAcceptNullBios);
		CPPUNIT_TEST(useEcho_RW);
	CPPUNIT_TEST_SUITE_END();

private:
	osock::BIO* b1;
	osock::BIO* b2;
public:
	void setUp(void)
	{
	    int sockfd, portno;
	    struct sockaddr_in serv_addr;
	    struct hostent *server;

	    portno = 7;	// echo server
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    assert (sockfd != 0);
	    server = gethostbyname("localhost");
	    assert(server != NULL);

	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    bcopy((char *)server->h_addr,
	         (char *)&serv_addr.sin_addr.s_addr,
	         server->h_length);
	    serv_addr.sin_port = htons(portno);

	    assert(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) == 0);

	    /* Attach the socket to a bio and choose autoclose for sock_fd */
	    BIO* bio = BIO_new_socket(sockfd, BIO_CLOSE);
		b1 = new osock::BIO(bio);
	}
	void tearDown(void)
	{
		delete b1;
	}

protected:
	void constructorsShallNotAcceptNullBios(void)
	{
		CPPUNIT_ASSERT_THROW(osock::BIO b(NULL, false), osock::ArgsException);
		CPPUNIT_ASSERT_THROW(osock::BIO b(NULL, true), osock::ArgsException);
	}
	void useEcho_RW(void)
	{
		std::string s("testpingmessage\r\n");
		osock::data_chunk d1(s.data(), s.data() + s.size());
		osock::data_chunk d2;

		b1->Write(d1);
		d2 = b1->Read();
		CPPUNIT_ASSERT_MESSAGE("message read shall be equal to written", d1 == d2);
	}
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BIOTest, Utils::getSuiteBIO());
CPPUNIT_REGISTRY_ADD(Utils::getSuiteBIO(), Utils::getSuiteGroupBIO());


static osock::data_chunk pingmessage;
class BIOSocket: public CPPUNIT_NS::TestCase
{
CPPUNIT_TEST_SUITE(BIOSocket);
		CPPUNIT_TEST(useEcho_RW_withRetry);
		CPPUNIT_TEST(useEcho_RW);
		CPPUNIT_TEST(acceptShallWork);
	CPPUNIT_TEST_SUITE_END();

private:
public:
	void setUp(void)
	{
		std::string s("MyPingMessage\r\n");
		pingmessage.clear();
		pingmessage.insert(pingmessage.begin(), s.data(), s.data() + s.size());
	}
	void tearDown(void)
	{
	}

protected:
	void useEcho_RW_withRetry(void)
	{
		osock::data_chunk d2;
		osock::BIOSocket_p c = osock::BIOSocket::PopulateClientBIO("localhost:echo") ;

		c->WriteWithRetry(pingmessage);
		d2 = c->ReadWithRetry();
		CPPUNIT_ASSERT_MESSAGE("when using echo server, ReadWithRetry() should return data sent in WriteWithRetry()", pingmessage == d2);
	}

	void useEcho_RW(void)
	{
		osock::data_chunk d2;
		osock::BIOSocket_p c = osock::BIOSocket::PopulateClientBIO("localhost:echo");

		c->setWriteTimeout(1000);
		c->setReadTimeout(1000);

		c->Write(pingmessage);
		d2 = c->Read();
		CPPUNIT_ASSERT_MESSAGE("when using echo server, Read() should return data sent in Write()", pingmessage == d2);
	}

	static void* pingerThread(void* port)
	{
		sleep(1);
		std::string host("localhost:");
		intptr_t portnum =(intptr_t)port;
		host += Utils::to_string(portnum);

		osock::BIOSocket_p c = osock::BIOSocket::PopulateClientBIO(host);
		c->setWriteTimeout(100);
		c->Write(pingmessage);

		pthread_exit(NULL);
	}

	void acceptShallWork() {
		pthread_t pinger;
		intptr_t port = Utils::getFreePort(10000,10000);
		osock::BIOSocket_p s = osock::BIOSocket::PopulateAcceptBIO(Utils::to_string(port));

		int res = pthread_create(&pinger, NULL, pingerThread, (void*) port);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("problems with pthread_create()", 0, res);
		pthread_detach(pinger);

		osock::BIOSocket_p c = s->AcceptIncoming();
		osock::data_chunk d = c->Read();
		CPPUNIT_ASSERT_MESSAGE("Data received from client shall be equal to sent", pingmessage == d);
	}
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BIOSocket, Utils::getSuiteBIOSocketClient());
CPPUNIT_REGISTRY_ADD(Utils::getSuiteBIOSocketClient(), Utils::getSuiteGroupBIO());

CPPUNIT_REGISTRY_ADD(Utils::getSuiteGroupBIO(), Utils::getSuiteGroupUnit());

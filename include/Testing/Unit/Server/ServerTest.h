/*
 * ServerTest.h
 *
 *  Created on: Jun 25, 2011
 *      Author: nazgee
 */

#ifndef SERVERTEST_H_
#define SERVERTEST_H_

#include <osock.h>
#include <testutils.h>
#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include <Testing/testutils.h>


class Challange
{
public:
	virtual bool Test(osock::SocketClient* client) = 0;
};

class ServerTest: public CPPUNIT_NS::TestCase
{

private:
	std::vector<Challange*> itsChallanges;
	osock::Server* itsServer;
	osock::SocketClient* itsClient;
	pthread_t server_thread;


protected:
	void installChallange(Challange* challange)
	{
		itsChallanges.insert(itsChallanges.end(), challange);
	}

	virtual osock::Server* setUpServer(osock::SocketServer* socketServer) = 0;
	virtual void setUpChallanges() = 0;

public:
	void virtual setUp(void)
	{
//		osock::Logger::ForceLoglevel(osock::Logger::logAll);
		int port = Utils::getFreePort(10000, 10000);

		std::string host;
		host = "localhost:" + Utils::to_string(port);
		osock::Address addr(host.c_str());
		itsServer = setUpServer(new osock::SocketServerUnsafe(port, osock::SocketServerUnsafe::serviceCallback));
		assert(itsServer != NULL);
        int s = pthread_create(&server_thread, NULL, threadServer, itsServer);
        CPPUNIT_ASSERT_MESSAGE("pthread problems; pthread_create()", s == 0);
		setUpChallanges();
		// sanity check
		itsClient = new osock::SocketClientUnsafe(addr);
	}

	void virtual tearDown(void)
	{
		delete itsClient;
		delete itsServer;
		itsServer = NULL;
		itsChallanges.clear();
	}
	/**
	 * @brief Starts server under test
	 * @param server pointer to server under test
	 * @return points to NULL on success, elsewhere on failure
	 */
	static void* threadServer(void* server)
	{
		osock::Server* srv = static_cast<osock::Server*>(server);

		srv->Run();
//		void* res = NULL;
//		try {
//			srv->Run();
//		} catch (...) {
//			// XXX: catch ONLY client disconnected exception
//			res = (void*)666;
//		}

		pthread_exit(NULL);
	}

	void serverShallRespondForRequests(void)
	{
		for (std::vector<Challange*>::iterator ic =
				itsChallanges.begin(); ic != itsChallanges.end(); ++ic) {
			CPPUNIT_ASSERT_MESSAGE("server should response for requests", (*ic)->Test(itsClient));
		}

		// TODO: make shutting server down easier
		pthread_cancel(server_thread);

        void* res;
        int s = pthread_join(server_thread, &res);

        std::cout << std::endl << std::endl << Utils::to_string(res) << std::endl;

        CPPUNIT_ASSERT_MESSAGE("pthread problems; pthread_join()", s == 0);
	}
};
#define SERVER_TESTS_PACK() \
	CPPUNIT_TEST(serverShallRespondForRequests)

#endif /* SERVERTEST_H_ */

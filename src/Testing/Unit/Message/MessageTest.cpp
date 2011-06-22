/*
 * MessageTest.cpp
 *
 *  Created on: Jun 17, 2011
 *      Author: nazgee
 */

#include <Testing/Message/MessageTest.h>

class MessageTest_http_Header: public MessageTest
{
	CPPUNIT_TEST_SUITE(MessageTest_http_Header);
	MESSAGE_TESTS_PACK();
	CPPUNIT_TEST_SUITE_END();
public:
	virtual void setUpTestDataValid()
	{
		std::string s;
		// simple message data
		s = "Header: Value" + osock::http::NEWLINE;
		installTestData(
				osock::data_chunk(&s.c_str()[0], &s.c_str()[s.length()]),
				TESTDATA_VALID);
		// complicated message data
		s = "Different Header: Different Value" + osock::http::NEWLINE;
		installTestData(
				osock::data_chunk(&s.c_str()[0], &s.c_str()[s.length()]),
				TESTDATA_VALID);
	}
	virtual void setUpTestDataInvalid()
	{
		std::string s;
		// malformed message data
		s = "Not Terminated Header: Not Terminated Value";
		installTestData(
				osock::data_chunk(&s.c_str()[0], &s.c_str()[s.length()]),
				TESTDATA_INVALID);
	}
	virtual void setUpMessagesToTest()
	{
		intstallTestMessage(new osock::http::Header());
	}
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MessageTest_http_Header, Utils::getSuiteNameUnit());

class MessageTest_http_Path: public MessageTest
{
	CPPUNIT_TEST_SUITE(MessageTest_http_Path);
	MESSAGE_TESTS_PACK();
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUpTestDataValid()
	{
		std::string s;
		// simple message data
		s = "index.html ";
		installTestData(
				osock::data_chunk(&s.c_str()[0], &s.c_str()[s.length()]),
				TESTDATA_VALID);
		// complicated message data
		s = "dir1/dir2/index.html ";
		installTestData(
				osock::data_chunk(&s.c_str()[0], &s.c_str()[s.length()]),
				TESTDATA_VALID);
	}
	virtual void setUpTestDataInvalid()
	{
		std::string s;
		// malformed message data
		s = "not_terminated.html";
		installTestData(
				osock::data_chunk(&s.c_str()[0], &s.c_str()[s.length()]),
				TESTDATA_INVALID);
	}
	virtual void setUpMessagesToTest()
	{
		intstallTestMessage(new osock::http::Path());
	}
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MessageTest_http_Path, Utils::getSuiteNameUnit());

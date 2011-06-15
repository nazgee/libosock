/*
 * StringMessageTest.cpp
 *
 *  Created on: Jun 15, 2011
 *      Author: nazgee
 */
#include <osock.h>
#include <testutils.h>

#include <iostream>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#define MSG_STRING "teststring"
#define MSG_TERMINATOR "testterminator"


class StringMessageTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(StringMessageTest);
  CPPUNIT_TEST(constructing);
  CPPUNIT_TEST(copying);
  CPPUNIT_TEST_SUITE_END();

  osock::StringMessage* m1_empty;
  osock::StringMessage* m2;
  osock::StringMessage* m3;
  osock::StringMessage* m4_binary;
  std::string MSG_BINSTRING;
  std::string MSG_BINTERMINATOR;

public:
  void setUp(void)
  {
	  char str[] = {1,2,3,0,5,6,7,0};
	  char term[] = {0,0,1,2,3,0,5,6,7,0};
	  MSG_BINSTRING.assign(str, sizeof(str));
	  MSG_BINSTRING.assign(term, sizeof(term));

	  m1_empty = new osock::StringMessage("", "");
	  m2 = new osock::StringMessage(MSG_STRING, MSG_TERMINATOR);
	  m3 = new osock::StringMessage(*m2);
	  m4_binary = new osock::StringMessage(MSG_BINSTRING, MSG_BINTERMINATOR);
  }
  void tearDown(void)
  {
	  delete m1_empty;
	  delete m2;
	  delete m3;
	  delete m4_binary;
  }

protected:
  void constructing(void)
  {
	  CPPUNIT_ASSERT(m2->getString() == MSG_STRING);
	  CPPUNIT_ASSERT(m2->getTerminator() == MSG_TERMINATOR);

	  CPPUNIT_ASSERT_EQUAL(MSG_BINTERMINATOR, m4_binary->getString());
	  CPPUNIT_ASSERT_EQUAL(MSG_BINSTRING, m4_binary->getString());
	  CPPUNIT_ASSERT_EQUAL(MSG_BINTERMINATOR, m4_binary->getTerminator());
  }
  void copying(void)
  {
	  CPPUNIT_ASSERT(*m2 == *m3);
	  CPPUNIT_ASSERT(m2->getString() == m3->getString());
	  CPPUNIT_ASSERT(m2->getTerminator() == m3->getTerminator());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StringMessageTest, Utils::getSuiteNameUnit());


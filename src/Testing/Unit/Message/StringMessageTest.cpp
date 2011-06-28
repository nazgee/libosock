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

class StringMessageTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(StringMessageTest);
  CPPUNIT_TEST(copiesShallBeEqual);
  CPPUNIT_TEST(constructorsArgumetsShallBeReturned);
  CPPUNIT_TEST_SUITE_END();

  osock::StringMessage* m_empty;
  osock::StringMessage* m_text;
  osock::StringMessage* m_text_copy;
  osock::StringMessage* m_binary;
  osock::StringMessage* m_binary_copy;
  std::string MSG_TXTSTRING;
  std::string MSG_TXTTERMINATOR;
  std::string MSG_BINSTRING;
  std::string MSG_BINTERMINATOR;

public:
  void setUp(void)
  {
	  char str[] = {1,2,3,0,5,6,7,0};
	  char term[] = {0,0,1,2,3,0,5,6,7,0};
	  MSG_BINSTRING.assign(str, sizeof(str));
	  MSG_BINSTRING.assign(term, sizeof(term));
	  MSG_TXTSTRING = "this is a text string";
	  MSG_TXTTERMINATOR= "this is a text terminator";

	  m_empty = new osock::StringMessage("", "");
	  m_text = new osock::StringMessage(MSG_TXTSTRING, MSG_TXTTERMINATOR);
	  m_text_copy = new osock::StringMessage(*m_text);
	  m_binary = new osock::StringMessage(MSG_BINSTRING, MSG_BINTERMINATOR);
	  m_binary_copy = new osock::StringMessage(*m_binary);
  }
  void tearDown(void)
  {
	  delete m_empty;
	  delete m_text;
	  delete m_text_copy;
	  delete m_binary;
	  delete m_binary_copy;
  }

protected:
  void copiesShallBeEqual(void)
  {
	  // Textual data
	  CPPUNIT_ASSERT_EQUAL_MESSAGE("values shall be equal", *m_text, *m_text_copy);
	  CPPUNIT_ASSERT_EQUAL_MESSAGE("terminators shall be equal", m_text->getTerminator(), m_text_copy->getTerminator());
	  CPPUNIT_ASSERT_EQUAL_MESSAGE("strings shall be equal", m_text->getString(), m_text_copy->getString());

	  // Binary data
	  CPPUNIT_ASSERT_EQUAL_MESSAGE("values shall be equal", *m_binary, *m_binary_copy);
	  CPPUNIT_ASSERT_EQUAL_MESSAGE("terminators shall be equal", m_binary->getTerminator(), m_binary_copy->getTerminator());
	  CPPUNIT_ASSERT_EQUAL_MESSAGE("strings shall be equal", m_binary->getString(), m_binary_copy->getString());
  }
  void constructorsArgumetsShallBeReturned(void)
  {
	  // Textual data
	  CPPUNIT_ASSERT_EQUAL(MSG_TXTSTRING, m_text->getString());
	  CPPUNIT_ASSERT_EQUAL(MSG_TXTTERMINATOR, m_text->getTerminator());

	  // Binary data
	  CPPUNIT_ASSERT_EQUAL(MSG_BINSTRING, m_binary->getString());
	  CPPUNIT_ASSERT_EQUAL(MSG_BINTERMINATOR, m_binary->getTerminator());
	  CPPUNIT_ASSERT_EQUAL(MSG_BINSTRING.length(), m_binary->getString().length());
	  CPPUNIT_ASSERT_EQUAL(MSG_BINTERMINATOR.length(), m_binary->getTerminator().length());
  }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StringMessageTest, Utils::getSuiteStringMessage());
CPPUNIT_REGISTRY_ADD(Utils::getSuiteStringMessage(), Utils::getSuiteGroupMessage());


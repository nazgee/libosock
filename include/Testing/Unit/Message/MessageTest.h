/*
 * MessageTest.h
 *
 *  Created on: Jun 17, 2011
 *      Author: nazgee
 */

#ifndef MESSAGETEST_H_
#define MESSAGETEST_H_

#include <osock.h>
#include <testutils.h>
#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

class MessageTest: public CPPUNIT_NS::TestCase
{

private:
	void tryPackUnpack(osock::Message* m, osock::data_chunk d, int iter = 3)
	{
		for (int i = 0; i < iter; ++i) {
			osock::data_chunk data(d);
			osock::data_chunk rest;
			do {
				osock::data_chunk unpacked;
				CPPUNIT_ASSERT_EQUAL_MESSAGE("message shall successfully deserialize", true, m->DeserializeChunk(data));
				rest = m->getDeserializingRemains();
				unpacked = m->Serialize();
				unpacked.insert(unpacked.end(), rest.begin(), rest.end()); //unpacked = unpacked + remains
				CPPUNIT_ASSERT_MESSAGE("unpacked message should equal data that was packed", unpacked == data);
				data = rest;
			} while (rest.size() > 0);
		}
	}

	std::vector<osock::data_chunk> itsTestDataValid;
	std::vector<osock::data_chunk> itsTestDataInvalid;
	std::vector<osock::Message*> itsMessagesToTest;
protected:
	enum testDataType_t {
		TESTDATA_VALID,
		TESTDATA_INVALID
	};

	void installTestData(const osock::data_chunk& data, enum testDataType_t type = TESTDATA_VALID)
	{
		switch (type) {
		case TESTDATA_VALID:
			itsTestDataValid.insert(itsTestDataValid.end(), data);
			break;
		case TESTDATA_INVALID:
			itsTestDataInvalid.insert(itsTestDataInvalid.end(), data);
			break;
		default:
			break;
		}
	}

	void intstallTestMessage(osock::Message* msg2test)
	{
		itsMessagesToTest.insert(itsMessagesToTest.end(), msg2test);
	}

	virtual void setUpTestDataValid() = 0;
	virtual void setUpTestDataInvalid() {}
	virtual void setUpMessagesToTest() = 0;

public:
	void virtual setUp(void)
	{
		// build test vectors
		setUpTestDataValid();
		setUpTestDataInvalid();
		setUpMessagesToTest();

		// do sanity checks on test vectors
	}

	void virtual tearDown(void)
	{
		// clear test vectors
		itsTestDataValid.clear();
		itsTestDataInvalid.clear();
		itsMessagesToTest.clear();
	}
	void packingSingleMesssages(void)
	{
		for (std::vector<osock::Message*>::iterator im =
				itsMessagesToTest.begin(); im != itsMessagesToTest.end(); ++im) {
			for (std::vector<osock::data_chunk>::iterator id =
					itsTestDataValid.begin(); id != itsTestDataValid.end(); ++id) {
				tryPackUnpack((*im), (*id));
			}
		}
	}

	void packingMultipleMesssages(void)
	{
		assert(itsTestDataValid.size() > 1);

		for (std::vector<osock::Message*>::iterator im =
				itsMessagesToTest.begin(); im != itsMessagesToTest.end(); ++im) {
			osock::data_chunk d;
			for (std::vector<osock::data_chunk>::iterator id =
					itsTestDataValid.begin(); id != itsTestDataValid.end(); ++id) {
				d.insert(d.end(), (*id).begin(), (*id).end());
			}

			tryPackUnpack((*im), d);
		}
	}
};

#define MESSAGE_TESTS_PACK() \
	CPPUNIT_TEST(packingSingleMesssages); \
	CPPUNIT_TEST(packingMultipleMesssages)
#endif /* MESSAGETEST_H_ */

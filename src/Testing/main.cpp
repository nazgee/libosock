/*
 * main.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: nazgee
 */

#include <testutils.h>

#include <iostream>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextTestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <osock.h>

int main(int ac, char **av)
{
	//--- Create the event manager and test controller
	CPPUNIT_NS::TestResult controller;

	//--- Add a listener that colllects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener(&result);

	//--- Add a listener that print brief info as test run.
	CPPUNIT_NS::BriefTestProgressListener brief;
	controller.addListener(&brief);

	//--- Add a listener that print dots as test run.
	CPPUNIT_NS::TextTestProgressListener progress;
	controller.addListener(&progress);

	//--- Add the top suite to the test runner
	CPPUNIT_NS::TextUi::TestRunner runner;

	if (ac > 2) {
		runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry(
					av[2]).makeTest());
	} else {
		runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry(
					Utils::getSuiteGroupUnit()).makeTest());
	}
	osock::Server::InstallChildReaper();
	if (ac > 1)
		osock::Logger::ForceLoglevel((osock::Logger::logLevel)atoi(av[1]));
	runner.run(controller);


	if (result.wasSuccessful()) {
		std::cout << "No failures encountered- nice one!" << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cout << "Oops!" << std::endl;
		//--- Output info about the failures
		CPPUNIT_NS::TextOutputter outputter(&result, std::cout);
		outputter.printFailures();

		return EXIT_FAILURE;
	}
}

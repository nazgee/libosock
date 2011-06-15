/*
 * defines.h
 *
 *  Created on: Jun 15, 2011
 *      Author: nazgee
 */

#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <iostream>

// XXX maybe make it a singleton, some time later?
class Utils
{
private:
	Utils() {}
public:
	static std::string getSuiteNameUnit() { return "Unit"; }
};

#endif /* TESTUTILS_H_ */

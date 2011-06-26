/*
 * defines.h
 *
 *  Created on: Jun 15, 2011
 *      Author: nazgee
 */

#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <iostream>
#include <sstream>

// XXX maybe make it a singleton, some time later?
class Utils
{
private:
	Utils() {}
public:
	static std::string getSuiteNameUnit() { return "Unit"; }
	template <class T>
	static std::string to_string (const T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}
};

#endif /* TESTUTILS_H_ */

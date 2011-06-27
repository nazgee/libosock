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
#include <stdlib.h>

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
	static unsigned int getRandom(unsigned int max)
	{
		// XXX this should initializes via Utils singleton
		static bool initialized = false;
		if (!initialized) {
			srand ( time(NULL) );
			initialized = true;
		}

		return rand() % (max + 1);
	}
};

#endif /* TESTUTILS_H_ */

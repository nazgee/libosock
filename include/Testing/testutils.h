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

	static unsigned int getFreePort(unsigned int port_first, unsigned int port_range)
	{
		int port_offset = Utils::getRandom(port_range);
		int port = port_first + port_offset;
		int retval;

		do {
			port_offset = (port_offset + 1) % port_range;
			port = port_first + port_offset;

			std::string cmd("netstat -ta | grep :" + Utils::to_string(port));
			retval = system(cmd.c_str());
//			std::cerr << "tried PORT=" << Utils::to_string(port)
//				<< "; cmd=" << cmd
//				<< "; res=" << Utils::to_string(retval)<< std::endl;
		} while (retval == 0);

		return port;
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

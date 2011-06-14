/*
 * main.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: nazgee
 */

#include <iostream>
#include <osock.h>

int main()
{
	osock::HttpResponseFile response("badfile.html");
	std::cout << "This will not happen if the file given above"
			"does not exist" << std::endl;
	return 0;
}

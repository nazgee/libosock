/*
 * AuthNone.cpp
 *
 *  Created on: Jul 3, 2011
 *      Author: nazgee
 */

#include <Security/AuthNone.h>
#include <Utilities/Logger.h>

static osock::Logger logger("AuthNone");

namespace osock
{

AuthNone::AuthNone()
{
	DBG_CONSTRUCTOR;
}

BIO_p AuthNone::Authenticate(BIO_p bio)
{
	DBG << "Dummy authenticating" << std::endl;
	return bio;
}

AuthNone_p AuthNone::Populate()
{
	AuthNone_p ret(new AuthNone());
	return ret;
}

}

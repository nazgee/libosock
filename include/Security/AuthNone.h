/*
 * AuthNone.h
 *
 *  Created on: Jul 3, 2011
 *      Author: nazgee
 */

#ifndef OSOCKAUTHNONE_H_
#define OSOCKAUTHNONE_H_

#include "../Security/Auth.h"
#include "../Security/SSLContext.h"

namespace osock
{

class AuthNone;
typedef boost::shared_ptr<AuthNone> AuthNone_p;

class AuthNone : public Auth
{
private:
	AuthNone();
public:
	virtual BIO_p Authenticate(BIO_p bio2auth);
	static AuthNone_p Populate();
};
}

#endif /* AUTHNONE_H_ */

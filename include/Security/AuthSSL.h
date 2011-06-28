/*
 * AuthSSL.h
 *
 *  Created on: Jul 3, 2011
 *      Author: nazgee
 */

#ifndef AUTHSSL_H_
#define AUTHSSL_H_

#include <Security/Auth.h>
#include <Security/SSLContext.h>

namespace osock
{

class AuthSSL;
typedef boost::shared_ptr<AuthSSL> AuthSSL_p;

class AuthSSL : public Auth
{
private:
	bool isClient;
	SSLContext_p itsContext;

	AuthSSL(bool clientmode, SSLContext_p context);
public:
	virtual BIO_p Authenticate(BIO_p bio2auth);

	static AuthSSL_p PopulateClientAuth(SSLContext_p context);
	static AuthSSL_p PopulateServerAuth(SSLContext_p context);
};
}


#endif /* AUTHSSL_H_ */

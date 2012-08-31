/*
 * Auth.h
 *
 *  Created on: Jul 2, 2011
 *      Author: nazgee
 */

#ifndef OSOCKAUTH_H_
#define OSOCKAUTH_H_
#include <boost/utility.hpp>
#include "../BIO/BIO.h"

namespace osock
{
class Auth;
typedef boost::shared_ptr<Auth> Auth_p;

class Auth : public boost::noncopyable
{
public:
	virtual BIO_p Authenticate(BIO_p bio2auth) = 0;
	virtual ~Auth() { }
};
}

#endif /* AUTH_H_ */

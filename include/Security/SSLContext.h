/*
 * Context.h
 *
 *  Created on: Jul 3, 2011
 *      Author: nazgee
 */

#ifndef SSLCONTEXT_H_
#define SSLCONTEXT_H_

#include <openssl/ssl.h>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

namespace osock
{
class SSLContext;
typedef boost::shared_ptr<SSLContext> SSLContext_p;

class SSLContext : boost::noncopyable
{
public:
	typedef enum
	{
		serverTLSv1,
#ifndef OPENSSL_NO_SSL2
		serverSSLv2,
#endif
		serverSSLv3,
		serverSSLv23,
		clientTLSv1,
#ifndef OPENSSL_NO_SSL2
		clientSSLv2,
#endif
		clientSSLv3,
		clientSSLv23
	} securityMode;
private:
	boost::shared_ptr<SSL_CTX> itsCTX;
	securityMode itsSecurityMode;
	std::string itsCertFile;
	std::string itsKeyFile;
	std::string itsTrustFile;
	std::string itsPassword;

	SSLContext(	securityMode method,
				std::string certFile,
				std::string keyFile,
				std::string trustFile,
				std::string password);

	static int PasswordCallback(char *buf, int size, int rwflag, void *userdata);
	void libsslInit();

public:
	~SSLContext();
	operator SSL_CTX* ();
	static SSLContext_p PopulateContext(securityMode method,
										std::string certFile = "",
										std::string keyFile = "",
										std::string trustFile = "",
										std::string password = "");

	static const SSL_METHOD* GetMethod(securityMode mode);
};
}

#endif /* SSLCONTEXT_H_ */

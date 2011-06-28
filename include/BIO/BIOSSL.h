/*
 * BIOSSL.h
 *
 *  Created on: Jul 3, 2011
 *      Author: nazgee
 */

#ifndef BIOSSL_H_
#define BIOSSL_H_

#include <BIO/BIO.h>
#include <Security/SSLContext.h>

namespace osock
{

class BIOSSL;
typedef boost::shared_ptr<BIOSSL> BIOSSL_p;

class BIOSSL : public BIO
{
private:
protected:
	explicit BIOSSL(::BIO* bio, bool closedescriptor = true, unsigned int chunksize = 30);
public:
	static BIOSSL_p PopulateClientBIOSSL(SSLContext_p context);
	static BIOSSL_p PopulateServerBIOSSL(SSLContext_p context);

	::SSL* getSSL();
	bool Handshake();
};
}


#endif /* BIOSSL_H_ */

/*
 * SocketBIO.h
 *
 *  Created on: Jun 28, 2011
 *      Author: nazgee
 */

#ifndef OSOCSOCKETBIO_H_
#define OSOCSOCKETBIO_H_

#include "../BIO/BIO.h"

#include <boost/shared_ptr.hpp>

namespace osock
{

class BIOSocket;
typedef boost::shared_ptr<BIOSocket> BIOSocket_p;

class BIOSocket : public BIO
{
public:
	void setOption(int level, int optnam, void* optvalue, socklen_t optlen);
	void getOption(int level, int optnam, void* optvalue, socklen_t& optlen);

	/**
	 * @brief Creates new accepting BIO listening for clients on a given port
	 *
	 * @param portname to listen at (e.g. "echo" or "7")
	 * @return new socket BIO that can start accepting connections on a given port
	 * @param chunksize maximumsize of data chunk to be returned with Read()
	 * operations
	 */
	static BIOSocket_p PopulateAcceptBIO(std::string portname, unsigned int chunksize = 30);
	/**
	 * @brief Creates a new client BIO connected to given host
	 * @param hostname name of the host to connect to, possibly in form
	 * 'host:port', where both, host and port can be string or numeric
	 * @param chunksize maximumsize of data chunk to be returned with Read()
	 * operations
	 */
	static BIOSocket_p PopulateClientBIO(std::string hostname, unsigned int chunksize = 30);
	/**
	 *
	 * @param bio OpenSSL BIO object to be managed by this object
	 * @param closedescriptor When set to false, socket BIO will NOT be closed
	 * on BIOSocket destruction
	 * @param chunksize maximum data_chunk size to be returned from Read() operations
	 */
	BIOSocket(::BIO* bio, bool closedescriptor = true, unsigned int chunksize = 30);
	virtual ~BIOSocket();
	/**
	 * @brief Sets timeout for Read() operations on a given socket BIO
	 * @param ms timeout value in milliseconds; when set to 0, Read() will block
	 * indefinitely
	 */
	void setReadTimeout(int ms);
	int getReadTimeout();
	/**
	 * @brief Sets timeout for Write() operations on a given socket BIO
	 * @param ms timeout value in milliseconds; when set to 0, Read() will block
	 * indefinitely
	 */
	void setWriteTimeout(int ms);
	/**
	 * @brief Starts listening for a client connections- returns client BIO when connected
	 * @return Client BIO that can be used to service connected client
	 */
	BIOSocket_p AcceptIncoming();
};

}

#endif /* SOCKETBIO_H_ */

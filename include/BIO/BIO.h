/*
	Copyright 2011, Michal Stawinski <michal.stawinski@gmail.com>

	This file is part of libsockets- C++ wrapper over OpenSSL and raw sockets.

	libsockets is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libsockets is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with libsockets.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BIO_H_
#define BIO_H_

#include "../Exception/Exception.h"

#include <defines.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace osock
{
class BIO;
typedef boost::shared_ptr<BIO> BIO_p;
typedef boost::weak_ptr<BIO> BIO_p_weak;

class BIO : public boost::noncopyable, public boost::enable_shared_from_this<BIO>
{
private:
	::BIO* itsBIO;
	unsigned int itsChunkSize;

	//! BIO added to this BIO using pushBIO()
	BIO_p itsNext;
	//! BIO to which this BIO was added using pushBIO()
	BIO_p_weak itsPrev;
protected:
	/**
	 * @brief Reads data from BIO; throws on failure (timeout/other)
	 *
	 * Returns data_chunk read from BIO. If an error is encountered,
	 * osock::StdException is thrown, containing errno.
	 * @return Data that was read from BIO
	 */
	data_chunk doRead() const;
	/**
	 * @brief Writes data to BIO; throws on failure (timeout/other)
	 *
	 * Writes data_chunk read to BIO. If an error is encountered,
	 * osock::StdException is thrown, containing errno.
	 * @return number of bytes successfully written
	 * @param data_to_write Data to be written
	 * @param offset Offset from which data_to_write should be written
	 */
	unsigned int doWrite(const data_chunk& data_to_write, int offset = 0) const;
	bool ShouldRetry();
public:
	/**
	 * @brief Constructor of BIO class- takes OpenSSL object
	 * @param bio
	 */
	explicit BIO(::BIO* bio, bool closedescriptor = true, unsigned int chunksize = 30);
	/**
	 * Destructor- it is virtual, to support polymorphism. Will free BIO
	 */
	virtual ~BIO();
	/**
	 * @brief Reads a data_chunk from BIO; throws on fatal error
	 * @return Data that was read from BIO- should always be of size() > 0
	 */
	virtual data_chunk Read() const;
	/**
	 * @brief Writes data_chunk to BIO; throws on fatal error
	 * @param data_to_write
	 */
	virtual data_chunk ReadWithRetry() const;
	/**
	 * @brief Writes data_chunk to BIO; throws on fatal error
	 * @param data_to_write
	 */
	virtual void Write(const data_chunk& data_to_write) const;
	/**
	 * @brief Reads a data_chunk from BIO; throws on fatal error
	 * @return Data that was read from BIO- should always be of size() > 0
	 */
	virtual void WriteWithRetry(const data_chunk& data_to_write) const;
	/**
	 * @brief Sets if file descriptor should be closed when osock::BIO
	 * destructor will be called
	 * @param closedescriptor when set to true, file will descriptor WILL be closed
	 * on osock::BIO destructor being called; whe set to false file descriptor
	 * will NOT be closed
	 */
	void setClose(bool closedescriptor);
	::BIO* getBIO() const;
	int getFD() const;
	unsigned int getChunkSize() const;
	void pushBIO(BIO_p bio);
	void popBIO();
	BIO_p getTopBIO();
	BIO_p getBottomBIO();
};

}

#endif /* BIO_H_ */

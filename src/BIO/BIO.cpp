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

#include <BIO/BIO.h>
#include <Utilities/Logger.h>
#include <Utilities/Utils.h>
static osock::Logger logger("BIO");

namespace osock
{

BIO::BIO(::BIO* bio, bool closedescriptor, unsigned int chunksize) :
	itsBIO(bio), itsChunkSize(chunksize)
{
	if (itsBIO == NULL) {
		throw ArgsException("NULL BIO given");
	}
//	setClose(closedescriptor);
	DBG_CONSTRUCTOR;
}

BIO::~BIO()
{
	DBG_DESTRUCTOR;
	BIO_free(itsBIO);
}

data_chunk BIO::doRead() const
{
	data_chunk tempData(itsChunkSize);

	DBG << "BIO read will be attempted" << std::endl;

	int rxedNumber = -1;
	while (rxedNumber < 0) {
		rxedNumber = BIO_read(itsBIO, &tempData.at(0), itsChunkSize);
		if (rxedNumber <= 0) {
			NFO << "BIO read interrupted" << std::endl;

			if (this->ShouldRetry()) {
				std::string s("Recoverable error when reading from BIO");
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					throw RecoverableException(s);
				}
			} else {
				std::string s("Fatal error when reading from BIO; ");
				s += Utils::getLastErrorSSL();
				throw RemoteDiedException(s);
			}
		} else {
			tempData.resize(rxedNumber);
			DBG << "Data read from BIO" << Utils::DataToString(tempData)
				<< std::endl;
		}
	}
	return tempData;
}

bool BIO::ShouldRetry() const
{
	return BIO_should_retry(this->getBIO());
}

unsigned int BIO::doWrite(const data_chunk& data_to_write, int offset) const
{
	int written = 0;
	int num2wr = data_to_write.size() - offset;
	assert(num2wr > 0);

	DBG << "Writing " << num2wr << "B - " << Utils::DataToString(data_to_write)
			<< std::endl;
	if (num2wr == 0) {
		return 0;
	}

	for (;;) {
		written = BIO_write(itsBIO, &data_to_write.at(offset), num2wr);
		if (written <= 0) {
			if (this->ShouldRetry()) {
				std::string s("Recoverable error when writing to BIO");
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					throw RecoverableException(s);
				}
			}

			std::string s("Fatal error when writing to BIO; ");
			s += Utils::getLastErrorSSL();
			throw RemoteDiedException(s);
		}
		break;
	}
	DBG << "Written " << written << "B" << std::endl;
	return written;
}

::BIO* BIO::getBIO() const
{
	return itsBIO;
}

int BIO::getFD() const
{
	int sd;
	long l = ::BIO_get_fd(itsBIO, &sd);
	if (l <= 0) {
		throw StdException("Could not get file descriptor for BIO");
	}

	return sd;
}

unsigned int BIO::getChunkSize() const
{
	return itsChunkSize;
}

data_chunk BIO::Read() const
{
	return doRead();
}

data_chunk BIO::ReadWithRetry() const
{
	for (;;) {
		try {
			return doRead();
		} catch (RecoverableException& ex) {

		}
	}
}

void BIO::Write(const data_chunk& data2wr) const
{
	unsigned int written = 0;
	do {
		data_chunk tmp;
		tmp.insert(tmp.end(), data2wr.begin() + written, data2wr.end());
		written += doWrite(tmp, written);
	} while(written < data2wr.size());
}

void BIO::WriteWithRetry(const data_chunk& data2wr) const
{
	unsigned int written = 0;
	do {
		try {
			data_chunk tmp;
			tmp.insert(tmp.end(), data2wr.begin() + written, data2wr.end());
			written += doWrite(tmp, written);
		} catch (RecoverableException& ex) {

		}
	} while (written < data2wr.size());
}

void BIO::setClose(bool closedescriptor)
{
	long l;
	if (closedescriptor)
		l = BIO_set_close(itsBIO, BIO_CLOSE);
	else
		l = BIO_set_close(itsBIO, BIO_NOCLOSE);

	if (l != 1)
		throw StdException("Could not set close mode for BIO");
}

void BIO::pushBIO(BIO_p bio)
{
	DBG << "Chaining " << this << "->" << bio << std::endl;
	assert(BIO_push(itsBIO, bio->getBIO()) != NULL);
	this->itsPrev = bio;
	bio->itsNext = shared_from_this();
}

void BIO::popBIO()
{
//	assert(BIO_pop(this->getBIO()) != NULL);

	// promote our previous BIO to fully-pledged smart pointer
	BIO_p prev = itsPrev.lock();
	// link lower and upper parts of chain together, so it stays not broken
	if (prev) {
		prev->itsNext = this->itsNext;
	}
	if (this->itsNext) {
		this->itsNext->itsPrev = prev;
	}
}

BIO_p BIO::getTopBIO()
{
	BIO_p next = shared_from_this();

	DBG << this;
	for(;;) {
		if (next->itsNext == NULL) {
			break;
		} else {
			next = next->itsNext;
			DBG << "<=" << next.get();
		}
	}
	DBG << ";" << std::endl;
	return next;
}

BIO_p BIO::getBottomBIO()
{
	BIO_p prev = shared_from_this();
	BIO_p tmp;

	DBG << this;
	for(;;) {
		tmp = prev->itsPrev.lock();
		if (tmp == NULL) {
			break;
		} else {
			prev = tmp;
			DBG << "->" << prev.get();
		}
	}
	DBG << ";" << std::endl;
	return prev;
}

}

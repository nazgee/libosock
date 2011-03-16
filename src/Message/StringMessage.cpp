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
#include <defines.h>
#include <Message/StringMessage.h>
#include <defines.h>

#include <string.h>
using namespace osock;

StringMessage::StringMessage(unsigned short dataLen)
{
	itsData = new char[dataLen];
	itsFreeSize = itsDataSize = dataLen;
	DBG_CONSTRUCTOR;
}

StringMessage::StringMessage(const char* data, int dataLen)
{
	if (dataLen <= 0)
		dataLen = strlen(data);

	dataLen++; //we need extra for NULL-terminator

	itsData = new char[dataLen];
	strncpy(itsData, data, dataLen - 1);
	itsFreeSize = itsDataSize = dataLen;
	DBG_CONSTRUCTOR;
}

StringMessage::~StringMessage(void)
{
	delete[] itsData;
	DBG_DESTRUCTOR;
}

char *StringMessage::Unpack(int& dataSize) const
{
	dataSize = itsDataSize;
	char *data = new char[itsDataSize];
	strcpy(data, itsData);
	return data;
}

bool StringMessage::Pack(char *data, int dataLen, int chunkNumber)
{
	if (chunkNumber > 1)
		ExpandData(data, dataLen);
	else
		SetData(data, dataLen);
	return (data[dataLen - 1] == 0);
}

char* StringMessage::GetData(void) const
{
	return itsData;
}

int StringMessage::GetDataSize(void) const
{
	return itsDataSize;
}

void StringMessage::SetDataSize(int dataLen)
{
	itsDataSize = dataLen;
}

int StringMessage::GetFreeData(void) const
{
	return itsFreeSize;
}

StringMessage& StringMessage::operator =(const char* data)
{
	SetData(data);
	return *this;
}

void StringMessage::SetData(const char* data, int dataLen)
{
	dataLen = strnlen(data, dataLen);

	//make sure we can store all bytes from data + NULL-terminator
	if (itsFreeSize <= dataLen) {
		delete[] itsData;
		itsData = new char[dataLen + 1];
		itsFreeSize = dataLen + 1;
	}
	itsDataSize = dataLen + 1;
	memcpy(itsData, data, dataLen);
	itsData[dataLen] = 0;
}

void StringMessage::ExpandData(const char* data, int dataLen)
{
	dataLen = strnlen(data, dataLen);

	if (itsDataSize + dataLen > itsFreeSize) {
		char *temp = new char[itsDataSize + dataLen];
		strcpy(temp, itsData);
		delete[] itsData;
		itsData = temp;
		itsFreeSize = itsDataSize + dataLen;
	}
	itsDataSize += dataLen;
	strncat(itsData, data, dataLen);
}

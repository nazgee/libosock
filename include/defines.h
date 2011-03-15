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

#ifndef DEFINES_H_
#define DEFINES_H_

#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <typeinfo>

#define UNUSED(x) (void)x

#undef DEBUG_FORCED_PRINTOUTS
#undef DEBUG_FORCED_CONSTRUCTORS
#undef DEBUG_FORCED_DESTRUCTORS
/*
 * DEBUG_FORCED_ZZZ - should be used only locally (here) - it will force
 * 		turning on given category of debugs GLOBALLY - regardless of
 * 		DEBUG_WANTED being defined or not.
 * 		ZZZ={PRINTOUTS, CONSTRUCTORS, DESTRUCTORS}
 *
 * DEBUG_WANTED - it should be defined in each modules that wants to turn on
 *	 	debug output
 */
//#define DEBUG_FORCED_PRINTOUTS
//#define DEBUG_FORCED_CONSTRUCTORS
//#define DEBUG_FORCED_DESTRUCTORS


#define LOG_SEPARATOR	" | "

//TODO demangle class name from typeid
#define LOGLINE_CLASS(outstream, header, separator) \
	outstream << header << typeid(*this).name() << "::" <<__func__ << " line:" << __LINE__ << separator

#define LOGLINE_FUNC(outstream, header, separator) \
	outstream << header << __func__ << " line:" << __LINE__ << separator

/* Sole purpose of this class is to discard all that goes into it */
struct NullStream: std::ostream
{
	struct nullbuf: std::streambuf
	{
		int overflow(int c)
		{
			return traits_type::not_eof(c);
		}
	} m_sbuf;
	NullStream() :
		std::ios(&m_sbuf), std::ostream(&m_sbuf)
	{
	}
};

//debugs logging
#if defined(DEBUG_WANTED) || defined(DEBUG_FORCED_PRINTOUTS)
#define DBG 			LOGLINE_CLASS(std::cout, "dbg   ", LOG_SEPARATOR)
#define DBG_FUNC		LOGLINE_FUNC(std::cout, "dbg   ", LOG_SEPARATOR)
#else
extern NullStream nullstream;
#define DBG 			LOGLINE_CLASS(nullstream, "dbg   ", LOG_SEPARATOR)
#define DBG_FUNC		LOGLINE_FUNC(nullstream, "dbg   ", LOG_SEPARATOR)
#endif
#if defined(DEBUG_WANTED) || defined(DEBUG_FORCED_CONSTRUCTORS)
#define DBG_CONSTRUCTOR	LOGLINE_FUNC(std::cout, "dbg  +", LOG_SEPARATOR) << this << std::endl
#else
extern NullStream nullstream;
#define DBG_CONSTRUCTOR	LOGLINE_FUNC(nullstream, "dbg  +", LOG_SEPARATOR) << this << std::endl
#endif
#if defined(DEBUG_WANTED) || defined(DEBUG_FORCED_DESTRUCTORS)
#define DBG_DESTRUCTOR	LOGLINE_FUNC(std::cout, "dbg  ", LOG_SEPARATOR) << this << std::endl
#else
extern NullStream nullstream;
#define DBG_DESTRUCTOR	LOGLINE_FUNC(nullstream, "dbg  ", LOG_SEPARATOR) << this << std::endl
#endif

//warnings logging
#define WRN 			LOGLINE_CLASS(std::cerr, "wrn ? ", LOG_SEPARATOR)
#define WRN_FUNC		LOGLINE_FUNC(std::cerr, "wrn ? ", LOG_SEPARATOR)

//errors logging
#define ERR 			LOGLINE_CLASS(std::cerr, "ERR ! ", LOG_SEPARATOR)
#define ERR_FUNC		LOGLINE_FUNC(std::cerr, "ERR ! ", LOG_SEPARATOR)

template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

#endif /* DEFINES_H_ */
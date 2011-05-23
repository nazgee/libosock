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
#ifndef LOGGER_H_
#define LOGGER_H_

#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <defines.h>

namespace osock
{
//=============================================================================
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
//=============================================================================
class Logger
{
private:
	Logger();
public:
	typedef enum { logOff, logDebug, logInfo, logWarn, logError } logLevel;

	static NullStream nullingstream;
	static logLevel printLoglevel;
	static std::ostream& Print(logLevel loglevel);
	virtual ~Logger();
};
//=============================================================================

#define LOGLEVEL_SURPRESS	LOGLEVEL_DBG
#define LOGLEVEL_FORCE		LOGLEVEL_WRN
#ifndef LOGLEVEL
#define LOGLEVEL 			LOGLEVEL_NFO
#endif

#define LOG_HEAD_DBG_CON "dbg  +"
#define LOG_HEAD_DBG_DES "dbg  ~"
#define LOG_HEAD_DBG "dbg   "
#define LOG_HEAD_NFO "nfo . "
#define LOG_HEAD_WRN "wrn ? "
#define LOG_HEAD_ERR "ERR ! "

#define LOGLEVEL_DBG_VAL	Logger::logDebug
#define LOGLEVEL_NFO_VAL	Logger::logInfo
#define LOGLEVEL_WRN_VAL	Logger::logWarn
#define LOGLEVEL_ERR_VAL	Logger::logError
#define LOGLEVEL_OFF_VAL	Logger::logOff
#define LOG_PRINT			Logger::Print

#define LOG_FSTRING		std::string(__PRETTY_FUNCTION__)
#define LOG_SEPARATOR	" | "
#define LOGFSTRING_MINE LOG_FSTRING.erase(LOG_FSTRING.find('(')).substr(LOG_FSTRING.find("::")+2)
//#define LOG_PRETTY_FUNC (std::string("                            ") + LOGFSTRING_MINE).substr(LOGFSTRING_MINE.length())
#define LOG_PRETTY_FUNC LOGFSTRING_MINE

#define LOG_FNAME_WIDTH 35

#define LOGLINE_CLASS(outstream, header, separator) \
	outstream << header << std::setw(LOG_FNAME_WIDTH) << std::left << LOG_PRETTY_FUNC << " line:" << std::setw(4) << __LINE__ << separator
#define LOGLINE_FUNC(outstream, header, separator) \
	outstream << header << std::setw(LOG_FNAME_WIDTH) << std::left << __FUNCTION__ << " line:" << std::setw(4) << __LINE__ << separator
#define LOGLINE_FUNC_NOLINE(outstream, header, separator) \
	outstream << header << std::setw(LOG_FNAME_WIDTH) << std::left << __FUNCTION__ << separator
#define LOGLEVEL_PASSED(value, level) (((value >= level) || (LOGLEVEL_FORCE >= level)) && (LOGLEVEL_SURPRESS >= level ))
//========================= debug logs =========================================
#if (LOGLEVEL_PASSED(LOGLEVEL, LOGLEVEL_DBG))
#define DBG 			LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_DBG_VAL), LOG_HEAD_DBG, LOG_SEPARATOR)
#define DBG_FUNC		LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_DBG_VAL), LOG_HEAD_DBG, LOG_SEPARATOR)
#define DBG_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(LOGLEVEL_DBG_VAL), LOG_HEAD_DBG, " ")
#define DBG_CONSTRUCTOR	LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_DBG_VAL), LOG_HEAD_DBG_CON, LOG_SEPARATOR) << this << std::endl
#define DBG_DESTRUCTOR	LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_DBG_VAL), LOG_HEAD_DBG_DES, LOG_SEPARATOR) << this << std::endl
#else
#define DBG 			LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_DBG, LOG_SEPARATOR)
#define DBG_FUNC		LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_DBG, LOG_SEPARATOR)
#define DBG_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_DBG, LOG_SEPARATOR)
//constructors and destructors have one last chance to still be printed
#if (defined(DBG_FORCED_CONSTRUCTORS))
#define DBG_CONSTRUCTOR	LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_DBG_VAL), LOG_HEAD_DBG_CON, LOG_SEPARATOR) << this << std::endl
#else
#define DBG_CONSTRUCTOR	LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_DBG_CON, LOG_SEPARATOR) << this << std::endl
#endif
#if (defined(DBG_FORCED_DESTRUCTORS))
#define DBG_DESTRUCTOR	LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_DBG_VAL), LOG_HEAD_DBG_DES, LOG_SEPARATOR) << this << std::endl
#else
#define DBG_DESTRUCTOR	LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_DBG_DES, LOG_SEPARATOR) << this << std::endl
#endif
#endif
//========================= info logs ==========================================
#if (LOGLEVEL_PASSED(LOGLEVEL, LOGLEVEL_NFO))
#define NFO 			LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_NFO_VAL), LOG_HEAD_NFO, LOG_SEPARATOR)
#define NFO_FUNC		LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_NFO_VAL), LOG_HEAD_NFO, LOG_SEPARATOR)
#define NFO_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(LOGLEVEL_NFO_VAL), LOG_HEAD_NFO, " ")
#else
#define NFO 			LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_NFO, LOG_SEPARATOR)
#define NFO_FUNC		LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_NFO, LOG_SEPARATOR)
#define NFO_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_NFO, LOG_SEPARATOR)
#endif
//========================= warning logs =======================================
#if (LOGLEVEL_PASSED(LOGLEVEL, LOGLEVEL_WRN))
#define WRN 			LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_WRN_VAL), LOG_HEAD_WRN, LOG_SEPARATOR)
#define WRN_FUNC		LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_WRN_VAL), LOG_HEAD_WRN, LOG_SEPARATOR)
#define WRN_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(LOGLEVEL_WRN_VAL), LOG_HEAD_WRN, " ")
#else
#define WRN 			LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_WRN, LOG_SEPARATOR)
#define WRN_FUNC		LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_WRN, LOG_SEPARATOR)
#define WRN_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_WRN, LOG_SEPARATOR)
#endif
//========================= error logs =========================================
#if (LOGLEVEL_PASSED(LOGLEVEL, LOGLEVEL_ERR))
#define ERR 			LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_ERR_VAL), LOG_HEAD_ERR, LOG_SEPARATOR)
#define ERR_FUNC		LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_ERR_VAL), LOG_HEAD_ERR, LOG_SEPARATOR)
#define ERR_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(LOGLEVEL_ERR_VAL), LOG_HEAD_ERR, " ")
#else
#define ERR 			LOGLINE_CLASS(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_ERR, LOG_SEPARATOR)
#define ERR_FUNC		LOGLINE_FUNC(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_ERR, LOG_SEPARATOR)
#define ERR_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(LOGLEVEL_OFF_VAL), LOG_HEAD_ERR, LOG_SEPARATOR)
#endif

}	//namespace osock

#endif /* LOGGER_H_ */

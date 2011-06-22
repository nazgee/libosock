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
#include <map>

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
public:
	typedef enum { logOff, logDebug, logInfo, logWarn, logError } logLevel;

private:
	typedef std::pair<std::string, logLevel> loggerDesc_t;
	std::string itsLogname;
	/**
	 * @brief Protects us from static initialization fiasco- Logger objects can
	 * be created stati, despite having static field
	 * @return Instance of loggers description map
	 */
	static std::map<std::string, logLevel>& getLoggers();
	/**
	 * @brief Protects us from static initialization fiasco- Logger objects can
	 * be created stati, despite having static field
	 * @return NullStream reference
	 */
	static NullStream& getNullStream();

public:
	Logger(std::string logname, int logelvel = -1);
	virtual ~Logger();

	std::ostream& out(logLevel loglevel);
};
//=============================================================================
#define LOG_SETTINGS_DBG_CON 	" dbg + "
#define LOG_SETTINGS_DBG_DES 	" dbg ~ "
#define LOG_SETTINGS_DBG 		" dbg   "
#define LOG_SETTINGS_NFO 		" nfo . "
#define LOG_SETTINGS_WRN 		" wrn ? "
#define LOG_SETTINGS_ERR 		" ERR ! "

#define LOG_PRINT			logger.out
#define LOG_FUNC_NAME_FORMAT std::setw(35) << std::left
#define LOG_FUNC_NAME_RAW	std::string(__PRETTY_FUNCTION__)
#define LOG_FUNC_NAME 		LOG_FUNC_NAME_RAW.erase(LOG_FUNC_NAME_RAW.find('(')).substr(LOG_FUNC_NAME_RAW.find("::")+2)
#define LOG_FUNC_LINE		"(l." << std::setw(4) << std::right << __LINE__ << ")"

#define LOGLINE_CLASS(outstream, header) \
	outstream << LOG_FUNC_LINE << LOG_FUNC_NAME_FORMAT << LOG_FUNC_NAME << header
#define LOGLINE_FUNC(outstream, header) \
	outstream << LOG_FUNC_LINE << LOG_FUNC_NAME_FORMAT << __FUNCTION__ << header
#define LOGLINE_FUNC_NOLINE(outstream, header) \
	outstream << LOG_FUNC_NAME_FORMAT << __FUNCTION__ << header
#define LOGLEVEL_PASSED(value, level) 1
//========================= debug logs =========================================
#define DBG_CONSTRUCTOR	LOGLINE_CLASS(LOG_PRINT(Logger::logDebug), LOG_SETTINGS_DBG_CON) << this << std::endl
#define DBG_DESTRUCTOR	LOGLINE_CLASS(LOG_PRINT(Logger::logDebug), LOG_SETTINGS_DBG_DES) << this << std::endl
#define DBG 			LOGLINE_CLASS(LOG_PRINT(Logger::logDebug), LOG_SETTINGS_DBG)
#define DBG_FUNC		LOGLINE_FUNC(LOG_PRINT(Logger::logDebug), LOG_SETTINGS_DBG)
#define DBG_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(Logger::logDebug), LOG_SETTINGS_DBG)
//========================= info logs ==========================================
#define NFO 			LOGLINE_CLASS(LOG_PRINT(Logger::logInfo), LOG_SETTINGS_NFO)
#define NFO_FUNC		LOGLINE_FUNC(LOG_PRINT(Logger::logInfo), LOG_SETTINGS_NFO)
#define NFO_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(Logger::logInfo), LOG_SETTINGS_NFO)
//========================= warning logs =======================================
#define WRN 			LOGLINE_CLASS(LOG_PRINT(Logger::logWarn), LOG_SETTINGS_WRN)
#define WRN_FUNC		LOGLINE_FUNC(LOG_PRINT(Logger::logWarn), LOG_SETTINGS_WRN)
#define WRN_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(Logger::logWarn), LOG_SETTINGS_WRN)
//========================= error logs =========================================
#define ERR 			LOGLINE_CLASS(LOG_PRINT(Logger::logError), LOG_SETTINGS_ERR)
#define ERR_FUNC		LOGLINE_FUNC(LOG_PRINT(Logger::logError), LOG_SETTINGS_ERR)
#define ERR_FUNC_NOLINE	LOGLINE_FUNC_NOLINE(LOG_PRINT(Logger::logError), LOG_SETTINGS_ERR)

}	//namespace osock

#endif /* LOGGER_H_ */

/**
 * MineDigger
 *
 * Copyright (C) 2015, Loïc Le Page
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Logger.h"

#include <cstdio>
#include <ctime>

#include <SDL2/SDL_log.h>

#ifdef _WIN32
#define GMTIME(A, B) !gmtime_s(B, A)
#elif defined __linux__
#define GMTIME(A, B) gmtime_r(A, B)
#else
#error Unsupported platform: currently supported platforms are\
       Linux and Windows
#endif //_WIN32

namespace
{
	//Size of a string buffer (including terminating \0 character) for ISO-8601
	//representation of UTC dates using the extended format.
	//ex: 2015-04-07T11:35:15Z
	const int DATE_BUFFER_SIZE = 21;

	//Maximum size of a log message (including terminating \0 character).
	const int LOG_MSG_SIZE = 2048;

	SDL_LogPriority convertLogPriority(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::INFO:
			return SDL_LOG_PRIORITY_INFO;

		case LogLevel::WARNING:
			return SDL_LOG_PRIORITY_WARN;

		case LogLevel::CRITICAL:
			return SDL_LOG_PRIORITY_CRITICAL;

		default:
			return SDL_LOG_PRIORITY_CRITICAL;
		}
	}
}

void Logger::log(LogLevel level, const char* format, ...) const
{
	if (format && (format[0] != '\0'))
	{
		std::va_list varArgs;
		va_start(varArgs, format);
		vlog(level, format, varArgs);
		va_end(varArgs);
	}
}

void Logger::info(const char* format, ...) const
{
	if (format && (format[0] != '\0'))
	{
		std::va_list varArgs;
		va_start(varArgs, format);
		vlog(LogLevel::INFO, format, varArgs);
		va_end(varArgs);
	}
}

void Logger::warning(const char* format, ...) const
{
	if (format && (format[0] != '\0'))
	{
		std::va_list varArgs;
		va_start(varArgs, format);
		vlog(LogLevel::WARNING, format, varArgs);
		va_end(varArgs);
	}
}

void Logger::critical(const char* format, ...) const
{
	if (format && (format[0] != '\0'))
	{
		std::va_list varArgs;
		va_start(varArgs, format);
		vlog(LogLevel::CRITICAL, format, varArgs);
		va_end(varArgs);
	}
}

void Logger::setLogPriority(LogLevel level)
{
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, convertLogPriority(level));
}

void Logger::vlog(LogLevel level, const char* format, std::va_list varArgs) const
{
	static char s_dateBuffer[DATE_BUFFER_SIZE];
	static char s_msgBuffer[LOG_MSG_SIZE];

	s_dateBuffer[0] = '\0';
	std::time_t timestamp = std::time(nullptr);
	if (timestamp != -1)
	{
		std::tm timeStruct = {};
		if (GMTIME(&timestamp, &timeStruct))
		{
			if (std::strftime(s_dateBuffer, DATE_BUFFER_SIZE, "%Y-%m-%dT%H:%M:%SZ", &timeStruct) != DATE_BUFFER_SIZE - 1)
				s_dateBuffer[0] = '\0';
		}
	}

	int nbChars = std::vsnprintf(s_msgBuffer, LOG_MSG_SIZE, format, varArgs);
	if (nbChars < 0)
		s_msgBuffer[0] = '\0';
	else if (nbChars >= LOG_MSG_SIZE)
		s_msgBuffer[LOG_MSG_SIZE - 1] = '\0';

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, convertLogPriority(level), "[%s][%s] %s", s_dateBuffer, m_moduleName, s_msgBuffer);
}

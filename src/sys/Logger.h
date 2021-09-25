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

#ifndef _LOGGER_H_
#define	_LOGGER_H_

#include <cstdarg>

namespace sys
{
	enum struct LogLevel : unsigned char
	{
		INFO,
		WARNING,
		CRITICAL
	};

	class Logger final
	{
	public:
		//WARNING: moduleName string content is not copied. The pointer is
		//directly stored internally, so moduleName memory buffer must have a
		//lifetime superior to the Logger instance. Normally, moduleName
		//should be a string literal set at compile-time. Common usage of
		//Logger class is as follows:
		//class MyClass
		//{
		//protected:
		//    static const Logger s_log;
		//};
		//
		//const Logger MyClass::s_log("MyClass");
		Logger(const char* moduleName)
		{
			if (moduleName)
				m_moduleName = moduleName;
		}

		void log(LogLevel level, const char* format, ...) const;
		void info(const char* format, ...) const;
		void warning(const char* format, ...) const;
		void critical(const char* format, ...) const;

		static void setLogPriority(LogLevel level);
		static LogLevel getLogPriority()
		{
			return s_logPriority;
		}

	private:
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		static LogLevel s_logPriority;

		const char* m_moduleName = "";

		void vlog(LogLevel level, const char* format, std::va_list varArgs) const;
	};
}

#endif //_LOGGER_H_

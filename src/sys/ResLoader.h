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

#ifndef _RESLOADER_H_
#define	_RESLOADER_H_

#include "Logger.h"

namespace sys
{
	class ResLoader final
	{
	public:
		static ResLoader* createResLoader();
		~ResLoader();

	private:
		ResLoader(std::size_t basePathLength, char* pPathBuffer) : m_basePathLength(basePathLength), m_pPathBuffer(pPathBuffer) {}
		ResLoader(const ResLoader&) = delete;
		ResLoader& operator=(const ResLoader&) = delete;

		const std::size_t m_basePathLength;
		char* const m_pPathBuffer;

		static const Logger s_log;

		//WARNING: returned value is temporary and only valid until next method
		//call or "asset" buffer lifetime
		const char* getAssetFullPath(const char* asset);
	};
}

#endif //_RESLOADER_H_

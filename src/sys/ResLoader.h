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

struct SDL_Surface;
struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;
struct Mix_Chunk;
struct _Mix_Music;
typedef struct _Mix_Music Mix_Music;

namespace sys
{
	class ResLoader final
	{
	public:
		static ResLoader* createResLoader();
		~ResLoader();

		SDL_Surface* loadImage(const char* asset);

		TTF_Font* loadFont(const char* asset, int pointSize, long fontFaceIdx);

		Mix_Chunk* loadSample(const char* asset);
		Mix_Music* loadMusic(const char* asset);

	private:
		ResLoader(std::size_t uBasePathLength, char* pPathBuffer) : m_uBasePathLength(uBasePathLength), m_pPathBuffer(pPathBuffer) {}
		ResLoader(const ResLoader&) = delete;
		ResLoader& operator=(const ResLoader&) = delete;

		const std::size_t m_uBasePathLength;
		char* const m_pPathBuffer;

		static const Logger s_log;

		//WARNING: returned value is temporary and only valid until next method
		//call or "asset" buffer lifetime
		const char* getAssetFullPath(const char* asset);
	};
}

#endif //_RESLOADER_H_

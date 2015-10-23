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

#ifndef _TEXTURE_H_
#define	_TEXTURE_H_

#include "Logger.h"

struct SDL_Texture;

namespace sys
{
	class GameEngine;

	class Texture final
	{
	public:
		static Texture* loadTexture(const char* asset, const GameEngine& engine);
		~Texture();

		const int m_width;
		const int m_height;

	private:
		Texture(int width, int height, SDL_Texture* pSDLTex) : m_width(width), m_height(height), m_pSDLTex(pSDLTex) {}
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		SDL_Texture* const m_pSDLTex;
		friend class Renderer;

		static const Logger s_log;
	};
}

#endif //_TEXTURE_H_

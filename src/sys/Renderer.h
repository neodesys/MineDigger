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

#ifndef _RENDERER_H_
#define	_RENDERER_H_

#include <cstdint>

#include "Logger.h"

struct SDL_Window;
struct SDL_Renderer;

namespace sys
{
	class IDrawable;

	class Renderer final
	{
	public:
		static Renderer* createRenderer(const char* wndName, int width, int height);
		~Renderer();

		bool setFullscreen(bool bFullscreen);
		bool isFullscreen() const
		{
			return m_bFullscreen;
		}

		void renderFrame(IDrawable* p);
		void clearBackground(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

	private:
		Renderer(SDL_Window* pSDLWnd, SDL_Renderer* pSDLRdr) : m_pSDLWnd(pSDLWnd), m_pSDLRdr(pSDLRdr) {}
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		SDL_Window* const m_pSDLWnd;
		SDL_Renderer* const m_pSDLRdr;

		bool m_bFullscreen = false;

		static const Logger s_log;
	};
}

#endif //_RENDERER_H_

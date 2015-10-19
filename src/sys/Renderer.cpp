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

#include "Renderer.h"

#include <cassert>
#include <new>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include "IDrawable.h"

namespace sys
{
	const Logger Renderer::s_log("Renderer");

	Renderer* Renderer::createRenderer(const char* wndName, int width, int height)
	{
		if (!wndName || (width < 1) || (height < 1))
		{
			s_log.critical("Invalid window parameters");
			return nullptr;
		}

		SDL_Window* pSDLWnd = SDL_CreateWindow(wndName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
		if (!pSDLWnd)
		{
			s_log.critical("Cannot create SDL2 window (%s)", SDL_GetError());
			return nullptr;
		}

		SDL_Renderer* pSDLRdr = SDL_CreateRenderer(pSDLWnd, -1, SDL_RENDERER_ACCELERATED);
		if (!pSDLRdr)
		{
			s_log.warning("Cannot create hardware accelerated SDL2 renderer (%s), falling back to software rendering...", SDL_GetError());

			pSDLRdr = SDL_CreateRenderer(pSDLWnd, -1, SDL_RENDERER_SOFTWARE);
			if (!pSDLRdr)
			{
				SDL_DestroyWindow(pSDLWnd);
				s_log.critical("Cannot create SDL2 renderer (%s)", SDL_GetError());
				return nullptr;
			}
		}

		Renderer* pRdr = new(std::nothrow) Renderer(pSDLWnd, pSDLRdr);
		if (!pRdr)
		{
			SDL_DestroyRenderer(pSDLRdr);
			SDL_DestroyWindow(pSDLWnd);
			s_log.critical("Out of memory");
			return nullptr;
		}

		s_log.info("Renderer created with dimensions %d x %d", width, height);
		return pRdr;
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(m_pSDLRdr);
		SDL_DestroyWindow(m_pSDLWnd);
	}

	bool Renderer::setFullscreen(bool bFullscreen)
	{
		if (bFullscreen == m_bFullscreen)
			return true;

		assert(m_pSDLWnd);
		if (bFullscreen)
		{
			if (SDL_SetWindowFullscreen(m_pSDLWnd, SDL_WINDOW_FULLSCREEN_DESKTOP))
			{
				s_log.warning("Cannot switch to fullscreen mode (%s)", SDL_GetError());
				return false;
			}

			m_bFullscreen = true;
			s_log.info("Switched to fullscreen mode");
		}
		else
		{
			if (SDL_SetWindowFullscreen(m_pSDLWnd, 0))
			{
				s_log.warning("Cannot switch to windowed mode (%s)", SDL_GetError());
				return false;
			}

			m_bFullscreen = false;
			s_log.info("Switched to windowed mode");
		}

		return true;
	}

	void Renderer::renderFrame(IDrawable* p)
	{
		if (p)
			p->draw(*this);
		else
			clearBackground(0, 0, 0, SDL_ALPHA_OPAQUE);

		assert(m_pSDLRdr);
		SDL_RenderPresent(m_pSDLRdr);
	}

	void Renderer::clearBackground(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		assert(m_pSDLRdr);
		if (SDL_SetRenderDrawColor(m_pSDLRdr, r, g, b, a))
			s_log.info("Cannot set background color (%s)", SDL_GetError());

		if (SDL_RenderClear(m_pSDLRdr))
			s_log.info("Cannot clear background (%s)", SDL_GetError());
	}
}

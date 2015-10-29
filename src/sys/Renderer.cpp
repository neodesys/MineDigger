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
#include <SDL2/SDL_hints.h>

#include "IDrawable.h"
#include "sys.h"
#include "Texture.h"

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

		SDL_Renderer* pSDLRdr = SDL_CreateRenderer(pSDLWnd, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
		if (!pSDLRdr)
		{
			s_log.warning("Cannot create hardware accelerated SDL2 renderer (%s), falling back to software rendering...", SDL_GetError());

			pSDLRdr = SDL_CreateRenderer(pSDLWnd, -1, SDL_RENDERER_SOFTWARE|SDL_RENDERER_PRESENTVSYNC);
			if (!pSDLRdr)
			{
				SDL_DestroyWindow(pSDLWnd);
				s_log.critical("Cannot create SDL2 renderer (%s)", SDL_GetError());
				return nullptr;
			}
		}

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
		if (SDL_RenderSetLogicalSize(pSDLRdr, width, height))
		{
			SDL_DestroyRenderer(pSDLRdr);
			SDL_DestroyWindow(pSDLWnd);
			s_log.critical("Cannot set SDL2 renderer logical size to %d x %d (%s)", width, height, SDL_GetError());
			return nullptr;
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
		assert(m_pSDLRdr);
		assert(m_pSDLWnd);

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

	SDL_Texture* Renderer::createSDLTextureFromSurface(SDL_Surface* pSDLSurf)
	{
		if (!pSDLSurf || (pSDLSurf->w < 1) || (pSDLSurf->h < 1))
		{
			s_log.warning("Cannot create SDL2 texture from a null or empty surface");
			return nullptr;
		}

		assert(m_pSDLRdr);
		SDL_Texture* pSDLTex = SDL_CreateTextureFromSurface(m_pSDLRdr, pSDLSurf);
		if (pSDLTex)
			return pSDLTex;

		s_log.warning("Cannot create SDL2 texture from surface (%s)", SDL_GetError());
		return nullptr;
	}

	void Renderer::renderFrame(IDrawable* p)
	{
		if (p)
			p->draw(*this);
		else
			clearBackground(nullptr);

		assert(m_pSDLRdr);
		SDL_RenderPresent(m_pSDLRdr);
	}

	void Renderer::clearBackground(const Color* pColor)
	{
		assert(m_pSDLRdr);

		if ((pColor && SDL_SetRenderDrawColor(m_pSDLRdr, pColor->r, pColor->g, pColor->b, SDL_ALPHA_OPAQUE)) ||
			(!pColor && SDL_SetRenderDrawColor(m_pSDLRdr, 0, 0, 0, SDL_ALPHA_OPAQUE)))
			s_log.warning("Cannot set background color (%s)", SDL_GetError());

		if (SDL_RenderClear(m_pSDLRdr))
			s_log.warning("Cannot clear background (%s)", SDL_GetError());
	}

	void Renderer::draw(IDrawable& d)
	{
		d.draw(*this);
	}

	void Renderer::drawTexture(const Texture& tex, const Rect* pSrcRect, const Rect* pDestRect)
	{
		assert(m_pSDLRdr);
		assert(tex.m_pSDLTex);

		if (SDL_RenderCopy(m_pSDLRdr, tex.m_pSDLTex, reinterpret_cast<const SDL_Rect*>(pSrcRect), reinterpret_cast<const SDL_Rect*>(pDestRect)))
			s_log.warning("Cannot draw texture (%s)", SDL_GetError());
	}
}

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

#include "Texture.h"

#include <cassert>
#include <new>

#include <SDL2/SDL_render.h>

#include "GameEngine.h"
#include "ResLoader.h"
#include "Renderer.h"
#include "sys.h"

namespace sys
{
	const Logger Texture::s_log("Texture");

	Texture* Texture::loadTexture(const GameEngine& engine, const char* asset)
	{
		SDL_Surface* pSDLSurf = engine.getResLoader().loadImage(asset);
		if (!pSDLSurf)
			return nullptr;

		SDL_Texture* pSDLTex = engine.getRenderer().createSDLTextureFromSurface(pSDLSurf);
		if (!pSDLTex)
		{
			SDL_FreeSurface(pSDLSurf);
			return nullptr;
		}

		Texture* pTex = new(std::nothrow) Texture(pSDLSurf->w, pSDLSurf->h, pSDLTex);
		SDL_FreeSurface(pSDLSurf);

		if (!pTex)
		{
			SDL_DestroyTexture(pSDLTex);
			s_log.critical("Out of memory");
			return nullptr;
		}

		s_log.info("Texture loaded for \"%s\"", asset);
		return pTex;
	}

	Texture::Texture(int width, int height, SDL_Texture* pSDLTex) :
		m_width(width), m_height(height), m_pSDLTex(pSDLTex)
	{
		assert(width >= 1);
		assert(height >= 1);
		assert(pSDLTex);
	}

	Texture::~Texture()
	{
		assert(m_pSDLTex);
		SDL_DestroyTexture(m_pSDLTex);
	}

	bool Texture::setTextureColorMod(const Color& color)
	{
		assert(m_pSDLTex);
		if (SDL_SetTextureColorMod(m_pSDLTex, color.r, color.g, color.b) ||
			SDL_SetTextureAlphaMod(m_pSDLTex, color.a))
		{
			s_log.warning("Cannot set texture modulation color (%s)", SDL_GetError());
			return false;
		}

		return true;
	}
}

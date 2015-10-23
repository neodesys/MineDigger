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

#include <new>

#include <SDL2/SDL_render.h>

#include "GameEngine.h"
#include "ResLoader.h"
#include "Renderer.h"

namespace sys
{
	const Logger Texture::s_log("Texture");

	Texture* Texture::loadTexture(const char* asset, const GameEngine& engine)
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

	Texture::~Texture()
	{
		SDL_DestroyTexture(m_pSDLTex);
	}
}

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

#include "Logger.h"

struct SDL_Texture;
struct SDL_Surface;
struct SDL_Window;
struct SDL_Renderer;

namespace sys
{
	class IDrawable;
	class Texture;
	class Rect;
	class Color;
	enum struct BlendMode : unsigned char;

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

		SDL_Texture* createSDLTextureFromSurface(SDL_Surface* pSDLSurf);

		void renderFrame(IDrawable* p);

		//Background is always cleared with an opaque color.
		//If pColor is null, background is cleared with opaque black.
		void clearBackground(const Color* pColor);

		void draw(IDrawable& d);

		//If pSrcRect is null, source texture is not clipped.
		//If pDestRect is null, destination is stretched to the entire viewport
		void drawTexture(const Texture& tex, const Rect* pSrcRect, const Rect* pDestRect);

		//If pDestRect is null, the entire viewport is painted
		void drawColoredRect(const Color& color, BlendMode mode, const Rect* pDestRect);

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

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

#include "Font.h"

#include <cassert>
#include <new>

#include <SDL2/SDL_ttf.h>

#include "GameEngine.h"
#include "ResLoader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Rect.h"

namespace sys
{
	const Logger Font::s_log("Font");

	Font* Font::loadFont(const GameEngine& engine, const char* asset, int pointSize, long fontFaceIdx)
	{
		if (pointSize < 1)
		{
			s_log.warning("Cannot create a font with a negative or null point size");
			return nullptr;
		}

		if (fontFaceIdx < 0)
			fontFaceIdx = 0;

		TTF_Font* pSDLFont = engine.getResLoader().loadFont(asset, pointSize, fontFaceIdx);
		if (!pSDLFont)
			return nullptr;

		Font* pFont = new(std::nothrow) Font(pointSize, pSDLFont);
		if (!pFont)
		{
			TTF_CloseFont(pSDLFont);
			s_log.critical("Out of memory");
			return nullptr;
		}

		s_log.info("Font loaded for \"%s\" with a size of %d pt", asset, pointSize);
		return pFont;
	}

	Font::~Font()
	{
		assert(m_pSDLFont);
		TTF_CloseFont(m_pSDLFont);
	}

	bool Font::isFontMonospace() const
	{
		assert(m_pSDLFont);
		return TTF_FontFaceIsFixedWidth(m_pSDLFont) ? true : false;
	}

	int Font::getFontHeight() const
	{
		assert(m_pSDLFont);
		return TTF_FontHeight(m_pSDLFont);
	}

	int Font::getFontAscent() const
	{
		assert(m_pSDLFont);
		return TTF_FontAscent(m_pSDLFont);
	}

	int Font::getFontDescent() const
	{
		assert(m_pSDLFont);
		return TTF_FontDescent(m_pSDLFont);
	}

	int Font::getFontLineSkip() const
	{
		assert(m_pSDLFont);
		return TTF_FontLineSkip(m_pSDLFont);
	}

	void Font::setFontHinting(Hinting hinting)
	{
		int sdlHinting = TTF_HINTING_NORMAL;
		switch (hinting)
		{
		case Hinting::NORMAL:
			break;

		case Hinting::LIGHT:
			sdlHinting = TTF_HINTING_LIGHT;
			break;

		case Hinting::MONO:
			sdlHinting = TTF_HINTING_MONO;
			break;

		case Hinting::NONE:
			sdlHinting = TTF_HINTING_NONE;
			break;
		}

		assert(m_pSDLFont);
		if (sdlHinting != TTF_GetFontHinting(m_pSDLFont))
			TTF_SetFontHinting(m_pSDLFont, sdlHinting);
	}

	Font::Hinting Font::getFontHinting() const
	{
		assert(m_pSDLFont);
		switch (TTF_GetFontHinting(m_pSDLFont))
		{
		case TTF_HINTING_LIGHT:
			return Hinting::LIGHT;

		case TTF_HINTING_MONO:
			return Hinting::MONO;

		case TTF_HINTING_NONE:
			return Hinting::NONE;

		default:
			return Hinting::NORMAL;
		}
	}

	void Font::setFontStyle(int style)
	{
		style &= STYLE_NORMAL|STYLE_BOLD|STYLE_ITALIC|STYLE_UNDERLINE|STYLE_STRIKETHROUGH;

		assert(m_pSDLFont);
		if (style != TTF_GetFontStyle(m_pSDLFont))
			TTF_SetFontStyle(m_pSDLFont, style);
	}

	int Font::getFontStyle() const
	{
		assert(m_pSDLFont);
		return TTF_GetFontStyle(m_pSDLFont);
	}

	void Font::setFontOutline(int outline)
	{
		if (outline < 0)
			outline = 0;

		assert(m_pSDLFont);
		if (outline != TTF_GetFontOutline(m_pSDLFont))
			TTF_SetFontOutline(m_pSDLFont, outline);
	}

	int Font::getFontOutline() const
	{
		assert(m_pSDLFont);
		return TTF_GetFontOutline(m_pSDLFont);
	}

	void Font::enableKerning(bool bEnable)
	{
		assert(m_pSDLFont);
		TTF_SetFontKerning(m_pSDLFont, bEnable ? 1 : 0);
	}

	bool Font::isKerningEnabled() const
	{
		assert(m_pSDLFont);
		return TTF_GetFontKerning(m_pSDLFont) ? true : false;
	}

	bool Font::getGlyphMetrics(char16_t glyph, Rect* pRect, int* pAdvance) const
	{
		assert(m_pSDLFont);
		if ((pRect && TTF_GlyphMetrics(m_pSDLFont, glyph, &pRect->x, &pRect->w, &pRect->h, &pRect->y, pAdvance)) ||
			(!pRect && TTF_GlyphMetrics(m_pSDLFont, glyph, nullptr, nullptr, nullptr, nullptr, pAdvance)))
		{
			s_log.warning("Cannot get 0x%04X unicode glyph metrics (%s)", glyph, TTF_GetError());
			return false;
		}

		if (pRect)
		{
			pRect->w -= pRect->x;
			pRect->h = pRect->y - pRect->h;
		}

		return true;
	}

	bool Font::getTextSize(const char* txt, int& w, int& h) const
	{
		if (!txt || (txt[0] == '\0'))
		{
			w = h = 0;
			return true;
		}

		assert(m_pSDLFont);
		if (TTF_SizeUTF8(m_pSDLFont, txt, &w, &h))
		{
			s_log.warning("Cannot get text size for \"%s\" (%s)", txt, TTF_GetError());
			return false;
		}

		return true;
	}

	Texture* Font::createTextTexture(const GameEngine& engine, const char* txt) const
	{
		if (!txt || (txt[0] == '\0'))
			return nullptr;

		assert(m_pSDLFont);
		SDL_Surface* pSDLSurf = TTF_RenderUTF8_Blended(m_pSDLFont, txt, {255, 255, 255, 255});
		if (!pSDLSurf)
		{
			s_log.warning("Cannot render \"%s\" text on surface (%s)", txt, TTF_GetError());
			return nullptr;
		}

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

		s_log.info("Texture created for \"%s\" text", txt);
		return pTex;
	}
}

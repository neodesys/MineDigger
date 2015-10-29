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

#include "NumberPrintRes.h"

#include <cassert>
#include <new>
#include <cmath>

#include "../sys/Font.h"
#include "../sys/Texture.h"
#include "../sys/Renderer.h"
#include "../sys/sys.h"

namespace app
{
	const sys::Logger NumberPrintRes::s_log("NumberPrintRes");

	NumberPrintRes* NumberPrintRes::createNumberPrintRes(const sys::GameEngine& engine, sys::Font& font)
	{
		//Compute each digit size
		int printAdvance = 0;
		if (!font.getGlyphMetrics(u'0', nullptr, &printAdvance))
			return nullptr;

		if (printAdvance <= 0)
		{
			s_log.warning("Glyph '0' has a null or negative width");
			return nullptr;
		}

		static int s_digitWidth[10] = {};
		s_digitWidth[0] = printAdvance;
		if (font.isFontMonospace())
		{
			//All digits have the same width
			for (int i = 1; i < 10; ++i)
				s_digitWidth[i] = printAdvance;
		}
		else
		{
			//Each digit may have a different width
			for (int i = 1; i < 10; ++i)
			{
				int advance = 0;
				if (!font.getGlyphMetrics(u'0' + i, nullptr, &advance))
					return nullptr;

				if (advance <= 0)
				{
					s_log.warning("Glyph '%c' has a null or negative width", '0' + i);
					return nullptr;
				}

				s_digitWidth[i] = advance;

				if (advance > printAdvance)
					printAdvance = advance;
			}
		}

		//Create digits texture
		bool bKerning = font.isKerningEnabled();
		if (bKerning)
			font.enableKerning(false);

		sys::Texture* pTexture = font.createTextTexture(engine, "0123456789");

		if (bKerning)
			font.enableKerning(true);

		if (!pTexture)
			return nullptr;

		NumberPrintRes* pPrintRes = new(std::nothrow) NumberPrintRes(pTexture);
		if (!pPrintRes)
		{
			delete pTexture;
			s_log.critical("Out of memory");
			return nullptr;
		}

		//Set digits data
		pPrintRes->m_printAdvance = printAdvance;
		pPrintRes->m_digitsHeight = font.getFontHeight();

		int offset = 0;
		for (int i = 0; i < 10; ++i)
		{
			pPrintRes->m_digitWidth[i] = s_digitWidth[i];
			pPrintRes->m_digitOffset[i] = offset;
			offset += s_digitWidth[i];
		}

		return pPrintRes;
	}

	NumberPrintRes::~NumberPrintRes()
	{
		assert(m_pTexture);
		delete m_pTexture;
	}

	void NumberPrintRes::getNumberPrintSize(unsigned int n, int minDigits, float scale, int& w, int& h) const
	{
		assert(m_pTexture);

		w = h = 0;
		if (scale > 0.f)
		{
			int nbChars = 0;
			while (n)
			{
				nbChars++;
				n /= 10;
			}

			if (nbChars < minDigits)
				nbChars = minDigits;

			if (nbChars > 0)
			{
				w = nbChars * static_cast<int>(std::round(scale * m_printAdvance));
				h = static_cast<int>(std::round(scale * m_digitsHeight));
			}
		}
	}

	void NumberPrintRes::printNumber(sys::Renderer& rdr, const sys::Rect& rect, const sys::Color& color, unsigned int n, int minDigits) const
	{
		assert(m_pTexture);
		if ((rect.w <= 0) || (rect.h <= 0) ||
			!m_pTexture->setTextureColorMod(color))
			return;

		int nbChars = 0;
		for (unsigned int u = n; u; u /= 10)
			nbChars++;

		if (nbChars < minDigits)
			nbChars = minDigits;

		if (nbChars <= 0)
			return;

		int destCharWidth = rect.w / nbChars;
		if (destCharWidth <= 0)
			return;

		int destOffset = rect.x + (nbChars - 1) * destCharWidth;

		sys::Rect srcRect = {0, 0, 0, m_digitsHeight};
		sys::Rect destRect = {0, rect.y, 0, rect.h};

		while (nbChars--)
		{
			unsigned char digit = n % 10;
			srcRect.x = m_digitOffset[digit];
			srcRect.w = m_digitWidth[digit];

			assert(m_printAdvance > 0);
			destRect.w = (srcRect.w * destCharWidth) / m_printAdvance;
			destRect.x = destOffset + ((destCharWidth - destRect.w) >> 1);

			rdr.drawTexture(*m_pTexture, &srcRect, &destRect);

			destOffset -= destCharWidth;
			n /= 10;
		}
	}
}

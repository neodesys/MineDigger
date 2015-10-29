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

#ifndef _NUMBERPRINTRES_H_
#define	_NUMBERPRINTRES_H_

#include "../sys/Logger.h"

namespace sys
{
	class GameEngine;
	class Font;
	class Renderer;
	class Rect;
	class Color;
	class Texture;
}

namespace app
{
	class NumberPrintRes final
	{
	public:
		static NumberPrintRes* createNumberPrintRes(const sys::GameEngine& engine, sys::Font& font);
		~NumberPrintRes();

		void getNumberPrintSize(unsigned int n, int minDigits, float scale, int& w, int& h) const;
		void printNumber(sys::Renderer& rdr, const sys::Rect& rect, const sys::Color& color, unsigned int n, int minDigits) const;

	private:
		NumberPrintRes(sys::Texture* pTexture) : m_pTexture(pTexture) {}
		NumberPrintRes(const NumberPrintRes&) = delete;
		NumberPrintRes& operator=(const NumberPrintRes&) = delete;

		sys::Texture* const m_pTexture;

		int m_digitOffset[10] = {};
		int m_digitWidth[10] = {};
		int m_digitsHeight = 0;
		int m_printAdvance = 0;

		static const sys::Logger s_log;
	};
}

#endif //_NUMBERPRINTRES_H_

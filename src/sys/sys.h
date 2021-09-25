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

#ifndef _SYS_H_
#define	_SYS_H_

#include <cmath>

namespace sys
{
	//WARNING: Rect MUST begin exactly as SDL_Rect structure.
	class Rect
	{
	public:
		Rect() = default;
		Rect(int x, int y) : x(x), y(y) {}
		Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
		Rect(float x, float y, float w, float h) :
			x(static_cast<int>(std::round(x))),
			y(static_cast<int>(std::round(y))),
			w(static_cast<int>(std::round(w))),
			h(static_cast<int>(std::round(h))) {}

		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
	};

	class Color
	{
	public:
		Color() = default;
		Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}

		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		unsigned char a = 255;
	};
}

#endif //_SYS_H_

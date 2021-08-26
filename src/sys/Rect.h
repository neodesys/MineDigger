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

#ifndef _RECT_H_
#define _RECT_H_

namespace sys
{
    // WARNING: Rect MUST begin exactly as SDL_Rect structure.
    class Rect
    {
      public:
        Rect() = default;
        Rect(int x, int y) : x(x), y(y)
        {
        }
        Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
        {
        }

        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };
} // namespace sys

#endif // _RECT_H_

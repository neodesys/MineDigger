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

#ifndef _IMOUSELISTENER_H_
#define _IMOUSELISTENER_H_

namespace sys
{
    class Vec2;

    class IMouseListener
    {
      public:
        virtual ~IMouseListener() = default;

        virtual void onMouseButtonDown(const Vec2& pos) = 0;
        virtual void onMouseButtonUp(const Vec2& pos) = 0;
        virtual void onMouseMove(const Vec2& pos, bool bDragging) = 0;
    };
} // namespace sys

#endif // _IMOUSELISTENER_H_

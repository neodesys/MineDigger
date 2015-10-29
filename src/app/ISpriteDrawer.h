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

#ifndef _ISPRITEDRAWER_H_
#define	_ISPRITEDRAWER_H_

namespace sys
{
	class Renderer;
	class Rect;
}

namespace app
{
	class ISpriteDrawer
	{
	public:
		virtual ~ISpriteDrawer() = default;

		virtual bool getSpriteSize(float scale, int& w, int& h) const = 0;
		virtual void drawSprite(sys::Renderer& rdr, const sys::Rect& rect) const = 0;
	};
}

#endif //_ISPRITEDRAWER_H_

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

#ifndef _SPRITE_H_
#define	_SPRITE_H_

#include "../sys/IDrawable.h"
#include "../sys/Vec2.h"

namespace app
{
	class ISpriteDrawer;

	class Sprite : public sys::IDrawable
	{
	public:
		void setPos(const sys::Vec2& pos)
		{
			m_pos = pos;
		}

		//Hotspot coordinates are relative to the sprite dimensions (between
		//0.f and 1.f). Thus, {0.f, 0.f} represents the sprite upper-left corner
		//and {1.f, 1.f} the sprite bottom-right corner. Default hotspot is
		//at sprite center: {0.5f, 0.5f}.
		void setHotspot(const sys::Vec2& hotspot)
		{
			m_hotspot = hotspot;
		}

		void setScale(float scale)
		{
			m_scale = scale;
		}

		void setSpriteDrawer(const ISpriteDrawer* pDrawer)
		{
			m_pDrawer = pDrawer;
		}

		virtual void draw(sys::Renderer& rdr) override;

	protected:
		sys::Vec2 m_pos;
		sys::Vec2 m_hotspot = {0.5f, 0.5f};
		float m_scale = 1.f;

	private:
		const ISpriteDrawer* m_pDrawer = nullptr;
	};
}

#endif //_SPRITE_H_

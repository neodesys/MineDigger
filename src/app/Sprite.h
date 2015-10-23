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

namespace sys
{
	class Texture;
}

namespace app
{
	class Sprite : public sys::IDrawable
	{
	public:
		void setPos(const sys::Vec2& pos)
		{
			m_pos = pos;
		}

		void setScale(float scale)
		{
			m_scale = scale;
		}

		void setTexture(const sys::Texture* pTexture);
		void setTexture(const sys::Texture* pTexture, const sys::Rect& clip);

		virtual void draw(sys::Renderer& rdr) override;

	protected:
		sys::Vec2 m_pos;
		float m_scale = 1.f;

		const sys::Texture* m_pTexture = nullptr;
		bool m_bSrcClip = false;
		sys::Rect m_srcClip;
	};
}

#endif //_SPRITE_H_

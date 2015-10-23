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

#include "Sprite.h"

#include "../sys/Renderer.h"
#include "../sys/Texture.h"

namespace app
{
	void Sprite::setTexture(const sys::Texture* pTexture)
	{
		m_pTexture = pTexture;
		m_bSrcClip = false;
		m_srcClip = {};
	}

	void Sprite::setTexture(const sys::Texture* pTexture, const sys::Rect& clip)
	{
		m_pTexture = pTexture;
		if (m_pTexture)
		{
			m_bSrcClip = true;
			m_srcClip = clip;
		}
		else
		{
			m_bSrcClip = false;
			m_srcClip = {};
		}
	}

	void Sprite::draw(sys::Renderer& rdr)
	{
		if (m_pTexture)
		{
			sys::Vec2 size;
			if (m_bSrcClip)
				size = {m_srcClip.w, m_srcClip.h};
			else
				size = {m_pTexture->m_width, m_pTexture->m_height};

			size *= m_scale;

			const sys::Rect destRect =
			{
				m_pos.x - 0.5f * size.x,
				m_pos.y - 0.5f * size.y,
				size.x,
				size.y
			};

			rdr.drawTexture(*m_pTexture, m_bSrcClip ? &m_srcClip : nullptr, &destRect);
		}
	}
}

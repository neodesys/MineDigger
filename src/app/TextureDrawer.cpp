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

#include "TextureDrawer.h"

#include <cmath>

#include "../sys/Texture.h"
#include "../sys/Renderer.h"

namespace app
{
	void TextureDrawer::setTexture(const sys::Texture* pTexture)
	{
		m_pTexture = pTexture;
		m_bSrcClip = false;
		m_srcClip = {};
	}

	void TextureDrawer::setTexture(const sys::Texture* pTexture, const sys::Rect& clip)
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

	bool TextureDrawer::getSpriteSize(float scale, int& w, int& h) const
	{
		if (!m_pTexture)
			return false;

		if (m_bSrcClip)
		{
			w = static_cast<int>(std::round(scale * m_srcClip.w));
			h = static_cast<int>(std::round(scale * m_srcClip.h));
		}
		else
		{
			w = static_cast<int>(std::round(scale * m_pTexture->m_width));
			h = static_cast<int>(std::round(scale * m_pTexture->m_height));
		}

		return true;
	}

	void TextureDrawer::drawSprite(sys::Renderer& rdr, const sys::Rect& rect) const
	{
		if (m_pTexture)
			rdr.drawTexture(*m_pTexture, m_bSrcClip ? &m_srcClip : nullptr, &rect);
	}
}

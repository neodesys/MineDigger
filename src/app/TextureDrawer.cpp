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

#include <cassert>
#include <cmath>

#include "../sys/Texture.h"
#include "../sys/Renderer.h"

namespace app
{
	void TextureDrawer::setTexture(const sys::Texture* pTexture, const sys::Rect* pClip)
	{
		m_pTexture = nullptr;
		m_bSrcClip = false;
		m_srcClip = {};

		m_uSubImagesCount = 0;
		m_uSubImagesStride = 0;
		m_uSelectedSubImage = 0;
		m_subImageClip = {};

		if (!pTexture)
			return;

		if (pClip)
		{
			m_srcClip = {0, 0, pTexture->m_width, pTexture->m_height};

			if (pClip->x > 0)
			{
				m_srcClip.x = pClip->x;
				m_srcClip.w -= pClip->x;
				m_bSrcClip = true;
			}

			if (pClip->y > 0)
			{
				m_srcClip.y = pClip->y;
				m_srcClip.h -= pClip->y;
				m_bSrcClip = true;
			}

			int l = pClip->x + pClip->w;
			if (l < m_srcClip.x + m_srcClip.w)
			{
				m_srcClip.w = l - m_srcClip.x;
				m_bSrcClip = true;
			}

			l = pClip->y + pClip->h;
			if (l < m_srcClip.y + m_srcClip.h)
			{
				m_srcClip.h = l - m_srcClip.y;
				m_bSrcClip = true;
			}

			if (!m_bSrcClip)
				m_srcClip = {};
			else if ((m_srcClip.w <= 0) || (m_srcClip.h <= 0))
			{
				m_srcClip = {};
				m_bSrcClip = false;
				return;
			}
		}

		m_pTexture = pTexture;
	}

	bool TextureDrawer::configureSubImages(unsigned int uCount, unsigned int uStride)
	{
		if (!uStride || (uCount < 2))
		{
			m_uSubImagesCount = 0;
			m_uSubImagesStride = 0;
			m_uSelectedSubImage = 0;
			m_subImageClip = {};
			return true;
		}

		if (!m_pTexture)
			return false;

		if (m_bSrcClip)
			m_subImageClip = m_srcClip;
		else
			m_subImageClip = {0, 0, m_pTexture->m_width, m_pTexture->m_height};

		m_subImageClip.w /= uStride;

		if (uCount % uStride)
			m_subImageClip.h /= (uCount / uStride) + 1;
		else
			m_subImageClip.h /= uCount / uStride;

		m_uSelectedSubImage = 0;

		if ((m_subImageClip.w <= 0) || (m_subImageClip.h <= 0))
		{
			m_uSubImagesCount = 0;
			m_uSubImagesStride = 0;
			m_subImageClip = {};
			return false;
		}

		m_uSubImagesCount = uCount;
		m_uSubImagesStride = uStride;

		return true;
	}

	bool TextureDrawer::selectSubImage(unsigned int uIndex)
	{
		if (uIndex >= m_uSubImagesCount)
			return false;

		if (uIndex == m_uSelectedSubImage)
			return true;

		assert(m_pTexture);
		assert(m_uSubImagesCount >= 2);
		assert(m_uSubImagesStride);
		assert(m_subImageClip.w > 0);
		assert(m_subImageClip.h > 0);

		if (m_bSrcClip)
		{
			m_subImageClip.x = m_srcClip.x;
			m_subImageClip.y = m_srcClip.y;
		}
		else
		{
			m_subImageClip.x = 0;
			m_subImageClip.y = 0;
		}

		m_subImageClip.x += (uIndex % m_uSubImagesStride) * m_subImageClip.w;
		m_subImageClip.y += (uIndex / m_uSubImagesStride) * m_subImageClip.h;

		m_uSelectedSubImage = uIndex;

		return true;
	}

	bool TextureDrawer::getSpriteSize(float scale, int& w, int& h) const
	{
		if (!m_pTexture)
			return false;

		if (m_uSubImagesCount)
		{
			assert(m_subImageClip.w > 0);
			assert(m_subImageClip.h > 0);

			w = static_cast<int>(std::round(scale * m_subImageClip.w));
			h = static_cast<int>(std::round(scale * m_subImageClip.h));
		}
		else if (m_bSrcClip)
		{
			assert(m_srcClip.w > 0);
			assert(m_srcClip.h > 0);

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
		{
			if (m_uSubImagesCount)
				rdr.drawTexture(*m_pTexture, &m_subImageClip, &rect);
			else if (m_bSrcClip)
				rdr.drawTexture(*m_pTexture, &m_srcClip, &rect);
			else
				rdr.drawTexture(*m_pTexture, nullptr, &rect);
		}
	}
}

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

#include "NumberDrawer.h"

#include "NumberStamp.h"
#include "../sys/Rect.h"

namespace app
{
	bool NumberDrawer::getSpriteSize(float scale, int& w, int& h) const
	{
		if (m_pNumberStamp)
		{
			m_pNumberStamp->getNumberPrintSize(m_uNumber, m_minDigits, scale, w, h);
			return true;
		}
		else
			return false;
	}

	void NumberDrawer::drawSprite(sys::Renderer& rdr, const sys::Rect& rect) const
	{
		if (m_pNumberStamp)
		{
			if (m_shadowColor.a)
			{
				sys::Rect shadowRect = {rect.x + m_shadowOffset[0], rect.y + m_shadowOffset[1], rect.w, rect.h};
				m_pNumberStamp->printNumber(rdr, shadowRect, m_shadowColor, m_uNumber, m_minDigits);
			}

			m_pNumberStamp->printNumber(rdr, rect, m_color, m_uNumber, m_minDigits);
		}
	}
}

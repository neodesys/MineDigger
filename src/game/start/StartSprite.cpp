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
#include "StartSprite.h"

namespace game
{
	namespace start
	{
		bool StartSprite::isLeftUpOut() const
		{
			int w = 0;
			int h = 0;
			if (m_textureDrawer.getSpriteSize(m_scale, w, h))
			{
				if ((m_pos.x + 0.5f * w < 0.f) || (m_pos.y + 0.5f * h < 0.f))
					return true;
			}

			return false;
		}
	}
}

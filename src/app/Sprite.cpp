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

#include "ISpriteDrawer.h"

namespace app
{
    void Sprite::draw(sys::Renderer& rdr)
    {
        if (m_pDrawer)
        {
            sys::Rect rect;
            if (m_pDrawer->getSpriteSize(m_scale, rect.w, rect.h))
            {
                rect.x = static_cast<int>(std::round(m_pos.x - m_hotspot.x * rect.w));
                rect.y = static_cast<int>(std::round(m_pos.y - m_hotspot.y * rect.h));
                m_pDrawer->drawSprite(rdr, rect);
            }
        }
    }
} // namespace app

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

#ifndef _STARTSPRITE_H_
#define _STARTSPRITE_H_

#include "../../app/DynSprite.h"
#include "../../app/TextureDrawer.h"

namespace game
{
    namespace start
    {
        class StartSprite : public app::DynSprite
        {
          public:
            StartSprite()
            {
                app::DynSprite::setSpriteDrawer(&m_textureDrawer);
            }

            void setTexture(const sys::Texture* pTexture, const sys::Rect* pClip = nullptr)
            {
                m_textureDrawer.setTexture(pTexture, pClip);
            }

            bool isLeftUpOut() const;

          private:
            StartSprite(const StartSprite&) = delete;
            StartSprite& operator=(const StartSprite&) = delete;

            // Forbid access to base methods
            void setHotspot(const sys::Vec2&) = delete;
            void setScale(float) = delete;
            void setSpriteDrawer(const app::ISpriteDrawer*) = delete;

            app::TextureDrawer m_textureDrawer;
        };
    } // namespace start
} // namespace game

#endif // _STARTSPRITE_H_

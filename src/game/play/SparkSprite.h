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

#ifndef _SPARKSPRITE_H_
#define _SPARKSPRITE_H_

#include "../../app/PathSprite.h"
#include "../../app/TextureDrawer.h"

namespace game
{
    namespace play
    {
        class SparkSprite final : public app::PathSprite
        {
          public:
            SparkSprite()
            {
                app::PathSprite::setSpriteDrawer(&m_textureDrawer);
            }

            void setTexture(sys::Texture* pTexture, unsigned int uSubImagesCount, unsigned int uSubImagesStride);
            void setImageAnimSpeed(float imgPerSec);

            void update(const sys::FrameInfo& frame) override final;

          private:
            SparkSprite(const SparkSprite&) = delete;
            SparkSprite& operator=(const SparkSprite&) = delete;

            // Forbid access to base methods
            void setAnimEndBehavior(app::AnimEndBehavior) = delete;
            void setAnimInterpolation(app::AnimInterpolation) = delete;
            void setPos(const sys::Vec2&) = delete;
            void setHotspot(const sys::Vec2&) = delete;
            void setScale(float) = delete;
            void setSpriteDrawer(const app::ISpriteDrawer*) = delete;

            app::TextureDrawer m_textureDrawer;
            unsigned int m_uImagePeriod = 80; // in ms
        };
    } // namespace play
} // namespace game

#endif // _SPARKSPRITE_H_

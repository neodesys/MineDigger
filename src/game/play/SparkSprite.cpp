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

#include "SparkSprite.h"

#include "../../sys/FrameInfo.h"
#include "../../sys/Texture.h"

namespace game
{
    namespace play
    {
        void SparkSprite::setTexture(sys::Texture* pTexture, unsigned int uSubImagesCount,
                                     unsigned int uSubImagesStride)
        {
            if (pTexture)
            {
                pTexture->setTextureBlendMode(sys::BlendMode::ADDITIVE);
                m_textureDrawer.setTexture(pTexture);
                m_textureDrawer.configureSubImages(uSubImagesCount, uSubImagesStride);
            }
            else
                m_textureDrawer.setTexture(nullptr);
        }

        void SparkSprite::setImageAnimSpeed(float imgPerSec)
        {
            if (imgPerSec > 0.f)
            {
                m_uImagePeriod = static_cast<unsigned int>(1000.f / imgPerSec);
                if (!m_uImagePeriod)
                    m_uImagePeriod = 1;
            }
            else
                m_uImagePeriod = 80;
        }

        void SparkSprite::update(const sys::FrameInfo& frame)
        {
            app::PathSprite::update(frame);

            if (m_bAnimRunning)
            {
                unsigned int uSubImagesCount = m_textureDrawer.getSubImagesCount();
                if (uSubImagesCount)
                {
                    unsigned long t = frame.getTimestamp();
                    if (t >= m_uAnimStartTimestamp)
                    {
                        t = (t - m_uAnimStartTimestamp) / m_uImagePeriod;
                        m_textureDrawer.selectSubImage(t % uSubImagesCount);
                    }
                }
            }
        }
    } // namespace play
} // namespace game

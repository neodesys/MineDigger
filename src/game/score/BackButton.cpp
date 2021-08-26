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

#include "ScoreScreen.h"

namespace game
{
    namespace score
    {
        BackButton::BackButton(ScoreScreen& scoreScreen) : m_scoreScreen(scoreScreen)
        {
            app::Sprite::setSpriteDrawer(&m_textureDrawer);
        }

        void BackButton::setOverlay(const sys::Texture* pTexture, const sys::Rect* pOverlayClip,
                                    unsigned int uOverlaySubImagesCount, unsigned int uOverlaySubImagesStride)
        {
            if (pTexture)
            {
                m_overlayDrawer.setTexture(pTexture, pOverlayClip);
                m_overlayDrawer.configureSubImages(uOverlaySubImagesCount, uOverlaySubImagesStride);
            }
            else
                m_overlayDrawer.setTexture(nullptr);

            m_bDrawOverlay = false;
        }

        void BackButton::onMouseButtonDown(const sys::Vec2& pos)
        {
            if (pos.isInside(m_backButtonRect))
            {
                m_overlayDrawer.selectSubImage(1);
                m_scoreScreen.playButtonSample();
            }
        }

        void BackButton::onMouseButtonUp(const sys::Vec2& pos)
        {
            if ((m_overlayDrawer.getSelectedSubImage() == 1) && pos.isInside(m_backButtonRect))
            {
                m_scoreScreen.playAgain();
                m_bDrawOverlay = false;
            }

            m_overlayDrawer.selectSubImage(0);
        }

        void BackButton::onMouseMove(const sys::Vec2& pos, bool bDragging)
        {
            m_bDrawOverlay = pos.isInside(m_backButtonRect);
        }

        void BackButton::draw(sys::Renderer& rdr)
        {
            if (m_bDrawOverlay)
            {
                sys::Rect rect;
                if (m_overlayDrawer.getSpriteSize(m_scale, rect.w, rect.h))
                {
                    rect.x = static_cast<int>(std::round(m_pos.x - m_hotspot.x * rect.w));
                    rect.y = static_cast<int>(std::round(m_pos.y - m_hotspot.y * rect.h));
                    m_overlayDrawer.drawSprite(rdr, rect);
                }
            }

            app::Sprite::draw(rdr);
        }
    } // namespace score
} // namespace game

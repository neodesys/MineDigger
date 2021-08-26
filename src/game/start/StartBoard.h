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

#ifndef _STARTBOARD_H_
#define _STARTBOARD_H_

#include "../../sys/IMouseListener.h"
#include "StartSprite.h"

namespace game
{
    namespace start
    {
        class StartScreen;

        class StartBoard final : public StartSprite, public sys::IMouseListener
        {
          public:
            StartBoard(StartScreen& startScreen) : m_startScreen(startScreen)
            {
            }

            void setOverlay(const sys::Texture* pTexture, const sys::Rect* pOverlayClip,
                            unsigned int uOverlaySubImagesCount, unsigned int uOverlaySubImagesStride);

            void setOverlayOffset(const sys::Vec2& offset)
            {
                m_overlayOffset = offset;
            }

            void setStartButtonRect(const sys::Rect& rect)
            {
                m_startButtonRect = rect;
            }

            void onMouseButtonDown(const sys::Vec2& pos) override final;
            void onMouseButtonUp(const sys::Vec2& pos) override final;
            void onMouseMove(const sys::Vec2& pos, bool bDragging) override final;

            void draw(sys::Renderer& rdr) override final;

          private:
            StartScreen& m_startScreen;

            bool m_bDrawOverlay = false;
            app::TextureDrawer m_overlayDrawer;
            sys::Vec2 m_overlayOffset;

            sys::Rect m_startButtonRect;
        };
    } // namespace start
} // namespace game

#endif // _STARTBOARD_H_

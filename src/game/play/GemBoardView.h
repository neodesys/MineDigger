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

#ifndef _GEMBOARDVIEW_H_
#define _GEMBOARDVIEW_H_

#include "../../sys/IMouseListener.h"
#include "GemSprite.h"

namespace game
{
    namespace play
    {
        class PlayScreen;

        class GemBoardView final : public sys::IMouseListener, public sys::IAnimated
        {
          public:
            class Config final
            {
              public:
                Config() = default;
                Config(int slotWidth, int slotHeight, int xPos, int yPos, int startLine, int endLine,
                       const GemSprite::Config& spriteConfig)
                    : slotWidth(slotWidth), slotHeight(slotHeight),
                      boardRect(xPos, yPos, slotWidth * GemBoardModel::NB_COLS, slotHeight * GemBoardModel::NB_ROWS),
                      startLine(startLine), endLine(endLine), spriteConfig(spriteConfig)
                {
                }

                int slotWidth = 0;  // Gem slot width
                int slotHeight = 0; // Gem slot height

                sys::Rect boardRect;

                int startLine = 0; // Gems will be placed above this line on init
                int endLine = 0;   // Thrown gems below this line will be recycled

                GemSprite::Config spriteConfig = {};
            };

            GemBoardView(PlayScreen& playScreen);

            void onMouseButtonDown(const sys::Vec2& pos) override final;
            void onMouseButtonUp(const sys::Vec2& pos) override final;
            void onMouseMove(const sys::Vec2& pos, bool bDragging) override final;

            void update(const sys::FrameInfo& frame) override final;

            void drawLayer(sys::Renderer& rdr, bool bFront);

            const PlayScreen& getPlayScreen() const
            {
                return m_playScreen;
            }

            const Config& getConfig() const
            {
                return m_config;
            }

            GemSprite* getSprite(int row, int col);

            int getDrawingPass() const
            {
                return m_drawingPass;
            }

            GemBoardModel& getModel()
            {
                return m_model;
            }

            // Model listener methods (must only be called by a GemBoardModel)
            void onBoardReset();
            void onCancelSwap(int rowA, int colA, int rowB, int colB);
            void onValidateSwap(int rowA, int colA, int rowB, int colB);
            void onGemCreated(GemType type, int row, int col);
            void onGemDestroyed(int row, int col);
            void onGemFallen(int oldRow, int newRow, int col);

          private:
            GemBoardView(const GemBoardView&) = delete;
            GemBoardView& operator=(const GemBoardView&) = delete;

            PlayScreen& m_playScreen;
            const Config& m_config;

            static constexpr int NB_SPRITES = GemBoardModel::NB_ROWS * GemBoardModel::NB_COLS * 2;
            GemSprite m_sprites[NB_SPRITES];

            GemSprite* m_pDraggedSprite = nullptr;
            GemSprite* m_pSelectedSprite = nullptr;
            void trySwapSelection();

            int m_drawingPass = 0;

            GemBoardModel m_model;
        };
    } // namespace play
} // namespace game

#endif // _GEMBOARDVIEW_H_

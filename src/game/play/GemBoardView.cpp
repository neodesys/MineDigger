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

#include "PlayScreen.h"

#include <cassert>

namespace game
{
    namespace play
    {
        GemBoardView::GemBoardView(PlayScreen& playScreen)
            : m_playScreen(playScreen), m_config(playScreen.getConfig().gemBoardConfig), m_model(*this)
        {
            for (int i = 0; i < NB_SPRITES; ++i)
                m_sprites[i].attachView(this);
        }

        void GemBoardView::onMouseButtonDown(const sys::Vec2& pos)
        {
            if (!pos.isInside(m_config.boardRect))
                return;

            int row = static_cast<int>(GemBoardModel::NB_ROWS - (pos.y - m_config.boardRect.y) / m_config.slotHeight);
            int col = static_cast<int>((pos.x - m_config.boardRect.x) / m_config.slotWidth);
            assert(!m_pDraggedSprite);

            GemSprite* pSprite = getSprite(row, col);
            if (pSprite && pSprite->select(pos))
            {
                m_pDraggedSprite = pSprite;
                trySwapSelection();
            }
        }

        void GemBoardView::onMouseButtonUp(const sys::Vec2& pos)
        {
            if (m_pDraggedSprite)
            {
                m_pDraggedSprite->unselect();
                m_pDraggedSprite = nullptr;
            }
        }

        void GemBoardView::onMouseMove(const sys::Vec2& pos, bool bDragging)
        {
            if (m_pDraggedSprite)
            {
                assert(bDragging);
                m_pDraggedSprite->setPos(pos);

                // Trigger a swap if we are too close to a neighbour
                sys::Vec2 offset = m_pDraggedSprite->getPos() - m_pDraggedSprite->getTargetPos();
                int row = m_pDraggedSprite->getModelRow();
                int col = m_pDraggedSprite->getModelCol();

                if ((row < GemBoardModel::NB_ROWS - 1) &&
                    (offset.distance({0, -m_config.slotHeight}) < 0.5f * m_config.slotHeight))
                {
                    // Try up
                    m_pSelectedSprite = getSprite(row + 1, col);
                    trySwapSelection();
                }
                else if ((row > 0) && (offset.distance({0, m_config.slotHeight}) < 0.5f * m_config.slotHeight))
                {
                    // Try down
                    m_pSelectedSprite = getSprite(row - 1, col);
                    trySwapSelection();
                }
                else if ((col < GemBoardModel::NB_COLS - 1) &&
                         (offset.distance({m_config.slotWidth, 0}) < 0.5f * m_config.slotWidth))
                {
                    // Try right
                    m_pSelectedSprite = getSprite(row, col + 1);
                    trySwapSelection();
                }
                else if ((col > 0) && (offset.distance({-m_config.slotWidth, 0}) < 0.5f * m_config.slotWidth))
                {
                    // Try left
                    m_pSelectedSprite = getSprite(row, col - 1);
                    trySwapSelection();
                }
            }
        }

        void GemBoardView::update(const sys::FrameInfo& frame)
        {
            for (GemSprite* p = m_sprites + NB_SPRITES - 1; p >= m_sprites; --p)
                p->update(frame);
        }

        void GemBoardView::drawLayer(sys::Renderer& rdr, bool bFront)
        {
            if (!bFront)
            {
                m_drawingPass = 0;
                for (GemSprite* p = m_sprites + NB_SPRITES - 1; p >= m_sprites; --p)
                    p->draw(rdr);

                m_drawingPass = 1;
                for (GemSprite* p = m_sprites + NB_SPRITES - 1; p >= m_sprites; --p)
                    p->draw(rdr);
            }
            else
            {
                m_drawingPass = 2;
                for (GemSprite* p = m_sprites + NB_SPRITES - 1; p >= m_sprites; --p)
                {
                    if (p != m_pDraggedSprite)
                        p->draw(rdr);
                }

                if (m_pDraggedSprite)
                    m_pDraggedSprite->draw(rdr);
            }
        }

        GemSprite* GemBoardView::getSprite(int row, int col)
        {
            if ((row >= 0) && (row < GemBoardModel::NB_ROWS) && (col >= 0) && (col < GemBoardModel::NB_COLS))
            {
                for (GemSprite* p = m_sprites + NB_SPRITES - 1; p >= m_sprites; --p)
                {
                    if ((p->getModelRow() == row) && (p->getModelCol() == col))
                        return p;
                }
            }

            return nullptr;
        }

        void GemBoardView::onBoardReset()
        {
            m_pDraggedSprite = m_pSelectedSprite = nullptr;

            int idx = 0;
            for (int row = 0; row < GemBoardModel::NB_ROWS; ++row)
            {
                for (int col = 0; col < GemBoardModel::NB_COLS; ++col)
                    m_sprites[idx++].initGem(m_model.getGem(row, col), row, col);
            }

            while (idx < NB_SPRITES)
                m_sprites[idx++].initGem(GemType::NO_GEM, -1, -1);
        }

        void GemBoardView::onCancelSwap(int rowA, int colA, int rowB, int colB)
        {
            assert((rowA >= 0) && (rowA < GemBoardModel::NB_ROWS));
            assert((colA >= 0) && (colA < GemBoardModel::NB_COLS));
            assert((rowB >= 0) && (rowB < GemBoardModel::NB_ROWS));
            assert((colB >= 0) && (colB < GemBoardModel::NB_COLS));

            bool bFirstFound = false;
            for (GemSprite* p = m_sprites + NB_SPRITES - 1; p >= m_sprites; --p)
            {
                if ((p->getModelRow() == rowA) && (p->getModelCol() == colA))
                    p->cancelMove();
                else if ((p->getModelRow() == rowB) && (p->getModelCol() == colB))
                    p->cancelMove();
                else
                    continue;

                if (p == m_pDraggedSprite)
                {
                    m_pDraggedSprite->unselect();
                    m_pDraggedSprite = nullptr;
                }

                if (p == m_pSelectedSprite)
                    m_pSelectedSprite = nullptr;

                if (bFirstFound)
                    break;

                bFirstFound = true;
            }

            sys::AudioSample* pSample = m_playScreen.getCancelSample();
            if (pSample)
                pSample->play();
        }

        void GemBoardView::onValidateSwap(int rowA, int colA, int rowB, int colB)
        {
            assert((rowA >= 0) && (rowA < GemBoardModel::NB_ROWS));
            assert((colA >= 0) && (colA < GemBoardModel::NB_COLS));
            assert((rowB >= 0) && (rowB < GemBoardModel::NB_ROWS));
            assert((colB >= 0) && (colB < GemBoardModel::NB_COLS));

            bool bFirstFound = false;
            for (GemSprite* p = m_sprites + NB_SPRITES - 1; p >= m_sprites; --p)
            {
                if ((p->getModelRow() == rowA) && (p->getModelCol() == colA))
                    p->validateMove(rowB, colB);
                else if ((p->getModelRow() == rowB) && (p->getModelCol() == colB))
                    p->validateMove(rowA, colA);
                else
                    continue;

                if (p == m_pDraggedSprite)
                {
                    m_pDraggedSprite->unselect();
                    m_pDraggedSprite = nullptr;
                }

                if (p == m_pSelectedSprite)
                    m_pSelectedSprite = nullptr;

                if (bFirstFound)
                    break;

                bFirstFound = true;
            }
        }

        void GemBoardView::onGemCreated(GemType type, int row, int col)
        {
            assert(type != GemType::NO_GEM);
            assert((row >= 0) && (row < GemBoardModel::NB_ROWS));
            assert((col >= 0) && (col < GemBoardModel::NB_COLS));

            GemSprite* pSprite = nullptr;
            for (GemSprite* p = m_sprites + NB_SPRITES - 1; p >= m_sprites; --p)
            {
                // We look for the first recycled GemSprite to use it as a new
                // gem
                if (p->getState() == GemSprite::State::RECYCLED)
                {
                    pSprite = p;
                    break;
                }

                // If there is no recycled GemSprite available (which is very
                // unlikely but may happen anyway), we look for the last
                // thrown-out sprite (which is becoming recycled at the end of
                // its animation in all cases)
                if (p->getState() == GemSprite::State::THROWN_OUT)
                    pSprite = p;
            }

            if (pSprite)
                pSprite->initGem(type, row, col);
        }

        void GemBoardView::onGemDestroyed(int row, int col)
        {
            assert((row >= 0) && (row < GemBoardModel::NB_ROWS));
            assert((col >= 0) && (col < GemBoardModel::NB_COLS));

            GemSprite* pSprite = getSprite(row, col);
            if (pSprite)
            {
                if (pSprite == m_pDraggedSprite)
                {
                    m_pDraggedSprite->unselect();
                    m_pDraggedSprite = nullptr;
                }

                if (pSprite == m_pSelectedSprite)
                    m_pSelectedSprite = nullptr;

                pSprite->throwGemOut();
                m_playScreen.addGemPoints();
            }
        }

        void GemBoardView::onGemFallen(int oldRow, int newRow, int col)
        {
            assert((oldRow >= 0) && (oldRow < GemBoardModel::NB_ROWS));
            assert((newRow >= 0) && (newRow < GemBoardModel::NB_ROWS));
            assert((col >= 0) && (col < GemBoardModel::NB_COLS));

            GemSprite* pSprite = getSprite(oldRow, col);
            if (pSprite)
            {
                if (pSprite == m_pDraggedSprite)
                {
                    m_pDraggedSprite->unselect();
                    m_pDraggedSprite = nullptr;
                }

                if (pSprite == m_pSelectedSprite)
                    m_pSelectedSprite = nullptr;

                pSprite->fallGem(newRow);
            }
        }

        void GemBoardView::trySwapSelection()
        {
            if (!m_pDraggedSprite)
                return;

            if (m_pSelectedSprite && (m_pSelectedSprite != m_pDraggedSprite))
            {
                int rowA = m_pDraggedSprite->getModelRow();
                int colA = m_pDraggedSprite->getModelCol();
                int rowB = m_pSelectedSprite->getModelRow();
                int colB = m_pSelectedSprite->getModelCol();

                assert((rowA >= 0) && (rowA < GemBoardModel::NB_ROWS));
                assert((colA >= 0) && (colA < GemBoardModel::NB_COLS));
                assert((rowB >= 0) && (rowB < GemBoardModel::NB_ROWS));
                assert((colB >= 0) && (colB < GemBoardModel::NB_COLS));

                if ((((rowA == rowB) && (std::abs(colB - colA) == 1)) ||
                     ((colA == colB) && (std::abs(rowB - rowA) == 1))) &&
                    (m_model.getGem(rowA, colA) != m_model.getGem(rowB, colB)) &&
                    m_pSelectedSprite->trySwapGem(rowA, colA))
                {
                    if (m_pDraggedSprite->trySwapGem(rowB, colB))
                    {
                        sys::AudioSample* pSample = m_playScreen.getSwapSample();
                        if (pSample)
                            pSample->play();
                    }
                    else
                    {
                        m_pSelectedSprite->cancelMove();
                        m_pDraggedSprite->unselect();
                    }

                    m_pDraggedSprite = nullptr;
                }
            }

            m_pSelectedSprite = m_pDraggedSprite;
        }
    } // namespace play
} // namespace game

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

#include "GemBoardView.h"
#include "PlayScreen.h"

#include <cassert>

namespace game
{
    namespace play
    {
        void GemSprite::setPos(const sys::Vec2& pos)
        {
            if (m_pView && m_bSelected)
            {
                assert((m_state == State::IN_PLACE) || (m_state == State::SPRING_ATTACHED));

                sys::Vec2 offset = pos + m_selectMouseOffset - m_targetPos;
                float dist = offset.norm();

                const Config& config = m_pView->getConfig().spriteConfig;
                if (dist >= config.minMoveOffset)
                {
                    if (dist > config.maxMoveOffset)
                        offset *= config.maxMoveOffset / dist;

                    m_pos = m_targetPos + offset;
                    m_state = State::SPRING_ATTACHED;
                }
            }
        }

        bool GemSprite::select(const sys::Vec2& mousePos)
        {
            if (m_pView && (m_state == State::IN_PLACE))
            {
                m_bSelected = true;
                m_selectMouseOffset = m_targetPos - mousePos;
                m_scale = m_pView->getConfig().spriteConfig.selectedSpriteScale;
                return true;
            }

            return false;
        }

        void GemSprite::unselect()
        {
            m_scale = 1.f;
            m_selectMouseOffset = {};
            m_bSelected = false;
        }

        void GemSprite::initGem(GemType type, int row, int col)
        {
            if (!m_pView)
                return;

            const sys::Texture* pTexture = m_pView->getPlayScreen().getGemTex(type);
            if (pTexture && (row >= 0) && (row < GemBoardModel::NB_ROWS) && (col >= 0) &&
                (col < GemBoardModel::NB_COLS))
            {
                m_textureDrawer.setTexture(pTexture);

                m_modelRow = row;
                m_modelCol = col;

                const GemBoardView::Config& config = m_pView->getConfig();

                m_targetPos.x = config.boardRect.x + (col + 0.5f) * config.slotWidth;
                m_targetPos.y = (config.boardRect.y + config.boardRect.h - 1) - (row + 0.5f) * config.slotHeight;

                m_pos.x = m_targetPos.x;
                m_pos.y = (config.startLine - 1) -
                          (config.spriteConfig.initRowSpacingFactor * row + 0.5f) * config.slotHeight;

                m_acceleration = {0.f, config.spriteConfig.gravityCoeff};
                m_state = State::FALLING;
            }
            else
            {
                m_textureDrawer.setTexture(nullptr);

                m_modelRow = m_modelCol = -1;
                m_targetPos = m_pos = m_acceleration = {};

                m_state = State::RECYCLED;
            }

            m_swapModelRow = m_swapModelCol = -1;
            m_scale = 1.f;
            m_moveVec = m_selectMouseOffset = {};
            m_bSelected = false;
        }

        bool GemSprite::trySwapGem(int row, int col)
        {
            if (m_pView && ((m_state == State::IN_PLACE) || (m_state == State::SPRING_ATTACHED)) &&
                ((row != m_modelRow) || (col != m_modelCol)) && ((m_swapModelRow < 0) || (m_swapModelCol < 0)) &&
                (row >= 0) && (row < GemBoardModel::NB_ROWS) && (col >= 0) && (col < GemBoardModel::NB_COLS))
            {
                unselect();
                m_swapModelRow = row;
                m_swapModelCol = col;

                const GemBoardView::Config& config = m_pView->getConfig();

                m_targetPos.x = config.boardRect.x + (col + 0.5f) * config.slotWidth;
                m_targetPos.y = (config.boardRect.y + config.boardRect.h - 1) - (row + 0.5f) * config.slotHeight;

                m_state = State::SPRING_ATTACHED;
                return true;
            }

            return false;
        }

        void GemSprite::cancelMove()
        {
            if (m_pView && ((m_state == State::IN_PLACE) || (m_state == State::SPRING_ATTACHED)))
            {
                unselect();
                m_swapModelRow = m_swapModelCol = -1;

                const GemBoardView::Config& config = m_pView->getConfig();

                m_targetPos.x = config.boardRect.x + (m_modelCol + 0.5f) * config.slotWidth;
                m_targetPos.y = (config.boardRect.y + config.boardRect.h - 1) - (m_modelRow + 0.5f) * config.slotHeight;

                m_state = State::SPRING_ATTACHED;
            }
        }

        void GemSprite::validateMove(int row, int col)
        {
            if (m_pView && ((m_state == State::IN_PLACE) || (m_state == State::SPRING_ATTACHED)) &&
                ((row != m_modelRow) || (col != m_modelCol)) && (row >= 0) && (row < GemBoardModel::NB_ROWS) &&
                (col >= 0) && (col < GemBoardModel::NB_COLS))
            {
                // WARNING: this method changes model row and col, so it
                // MUST ALWAYS be called symetrically on the other swapped
                // GemSprite to prevent duplicated indices

                unselect();
                m_swapModelRow = m_modelRow = row;
                m_swapModelCol = m_modelCol = col;

                const GemBoardView::Config& config = m_pView->getConfig();

                m_targetPos.x = config.boardRect.x + (col + 0.5f) * config.slotWidth;
                m_targetPos.y = (config.boardRect.y + config.boardRect.h - 1) - (row + 0.5f) * config.slotHeight;

                m_state = State::SPRING_ATTACHED;
            }
        }

        void GemSprite::fallGem(int lowerRow)
        {
            if (m_pView &&
                ((m_state == State::FALLING) || (m_state == State::IN_PLACE) || (m_state == State::SPRING_ATTACHED)) &&
                (lowerRow < m_modelRow) && (lowerRow >= 0) && (lowerRow < GemBoardModel::NB_ROWS))
            {
                assert(!m_pView->getSprite(lowerRow, m_modelCol));

                interruptSwap();
                unselect();
                m_modelRow = lowerRow;

                const GemBoardView::Config& config = m_pView->getConfig();
                m_targetPos.y = (config.boardRect.y + config.boardRect.h - 1) - (lowerRow + 0.5f) * config.slotHeight;

                if (m_state != State::FALLING)
                {
                    m_pos.x = m_targetPos.x;
                    m_moveVec = {};
                    m_acceleration = {0.f, config.spriteConfig.gravityCoeff};
                    m_state = State::FALLING;
                }
            }
        }

        void GemSprite::throwGemOut()
        {
            if (m_pView &&
                ((m_state == State::FALLING) || (m_state == State::IN_PLACE) || (m_state == State::SPRING_ATTACHED)))
            {
                interruptSwap();
                unselect();
                m_modelRow = m_modelCol = -1;

                const GemBoardView::Config& config = m_pView->getConfig();

                m_moveVec.x = 0.f;
                m_moveVec.y = -config.slotHeight * config.spriteConfig.thrownOutImpulseCoeff;

                m_acceleration = {0.f, config.spriteConfig.gravityCoeff};

                m_targetPos.y = config.endLine + 0.5f * config.slotHeight;
                m_state = State::THROWN_OUT;
            }
        }

        void GemSprite::update(const sys::FrameInfo& frame)
        {
            if (!m_pView || m_bSelected)
                return;

            switch (m_state)
            {
            case State::FALLING:
                app::DynSprite::update(frame);
                if (m_pos.y > m_targetPos.y)
                {
                    m_pos = m_targetPos;
                    m_moveVec = m_acceleration = {};
                    m_state = State::IN_PLACE;

                    m_pView->getModel().tryCollapse(m_modelRow, m_modelCol);
                }
                break;

            case State::SPRING_ATTACHED: {
                sys::Vec2 offset = m_targetPos - m_pos;
                float dist = offset.norm();
                if (dist >= 1.f)
                {
                    const Config& config = m_pView->getConfig().spriteConfig;

                    m_acceleration = offset * config.anchorSpringConstant;

                    // We project m_moveVec on offset vector direction in
                    // order to keep movement in line
                    offset *= 1.f / dist;
                    m_moveVec = offset * m_moveVec.dot(offset);

                    m_acceleration -= m_moveVec * config.anchorSpringDampingCoeff;
                    app::DynSprite::update(frame);

                    if (offset.dot(m_targetPos - m_pos) >= 1.f)
                        return;
                }

                m_pos = m_targetPos;
                m_moveVec = m_acceleration = {};

                if ((m_swapModelRow < 0) || (m_swapModelCol < 0))
                    m_state = State::IN_PLACE;
                else
                {
                    // We've reached destination place during a swap
                    // procedure. Let's perform next step...

                    if ((m_swapModelRow == m_modelRow) && (m_swapModelCol == m_modelCol))
                    {
                        // We are right after a swap success, it's time to
                        // try collapsing gems
                        m_swapModelRow = m_swapModelCol = -1;
                        m_state = State::IN_PLACE;

                        m_pView->getModel().tryCollapse(m_modelRow, m_modelCol);
                    }
                    else
                    {
                        // We are right after a try-swap, let's ask model to
                        // perform the swap procedure
                        m_pView->getModel().swapGems(m_modelRow, m_modelCol, m_swapModelRow, m_swapModelCol);

                        // During the swap procedure, the model triggers
                        // an onValidateSwap or onCancelSwap event which is
                        // caught by the view. Then the view validates or
                        // cancels swap for each of both gems. Therefore,
                        // the model swapGems() method is only called by the
                        // first gem reaching its place, after this call both
                        // gems are in their previous state (move canceled),
                        // or waiting for a collapsing try (move validated).

                        assert(m_state == State::SPRING_ATTACHED);
                        assert((m_swapModelRow < 0) || (m_swapModelCol < 0) ||
                               ((m_swapModelRow == m_modelRow) && (m_swapModelCol == m_modelCol)));
                    }
                }
            }
            break;

            case State::THROWN_OUT:
                app::DynSprite::update(frame);
                if (m_pos.y > m_targetPos.y)
                {
                    m_pos = m_targetPos = m_moveVec = m_acceleration = {};
                    m_textureDrawer.setTexture(nullptr);
                    m_state = State::RECYCLED;
                }
                break;

            default:
                break;
            }
        }

        void GemSprite::draw(sys::Renderer& rdr)
        {
            if (!m_pView)
                return;

            switch (m_pView->getDrawingPass())
            {
            case 0:
                if ((m_state == State::FALLING) || (m_state == State::IN_PLACE))
                    app::DynSprite::draw(rdr);
                break;

            case 1:
                if (m_state == State::THROWN_OUT)
                    app::DynSprite::draw(rdr);
                break;

            case 2:
                if (m_state == State::SPRING_ATTACHED)
                    app::DynSprite::draw(rdr);
                break;
            }
        }

        void GemSprite::interruptSwap()
        {
            if ((m_swapModelRow < 0) || (m_swapModelCol < 0))
                return;

            // A swap procedure is running
            if ((m_swapModelRow == m_modelRow) && (m_swapModelCol == m_modelCol))
            {
                // Swap has already been processed by the model, we are waiting
                // for a collapsing try which has become useless
                m_swapModelRow = m_swapModelCol = -1;
            }
            else
            {
                // Swap has not been processed by the model yet, so we have to
                // cancel swap for this gem and its swapping pair
                assert(m_pView);

                GemSprite* pPair = m_pView->getSprite(m_swapModelRow, m_swapModelCol);
                if (pPair)
                    pPair->cancelMove();

                cancelMove();
            }
        }
    } // namespace play
} // namespace game

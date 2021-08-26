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

#include "ScoreSprite.h"

#include "../../sys/FrameInfo.h"

namespace game
{
    namespace score
    {
        void ScoreSprite::startAnim(unsigned long uAnimDuration)
        {
            if (uAnimDuration)
            {
                m_scoreSpeed = 1000.f / uAnimDuration;

                m_moveSpeed = m_finalPos - m_pos;
                m_moveSpeed *= m_scoreSpeed;

                m_scoreSpeed *= m_uFinalScore;

                m_currentScore = 0.f;
                m_numberDrawer.setNumber(0);
            }
            else
            {
                m_scoreSpeed = 0.f;
                m_moveSpeed = {};

                m_currentScore = static_cast<float>(m_uFinalScore);
                m_pos = m_finalPos;
                m_numberDrawer.setNumber(m_uFinalScore);
            }
        }

        void ScoreSprite::update(const sys::FrameInfo& frame)
        {
            // Update score
            if (m_currentScore < m_uFinalScore)
            {
                m_currentScore += frame.getDuration() * m_scoreSpeed;
                if (m_currentScore < m_uFinalScore)
                    m_numberDrawer.setNumber(static_cast<unsigned int>(m_currentScore));
                else
                {
                    m_currentScore = static_cast<float>(m_uFinalScore);
                    m_numberDrawer.setNumber(m_uFinalScore);
                }
            }

            // Update position
            sys::Vec2 offset = m_finalPos - m_pos;
            float dist = offset.norm();
            if (dist >= 1.f)
            {
                m_pos += m_moveSpeed * frame.getDuration();

                offset *= 1.f / dist;
                if (offset.dot(m_finalPos - m_pos) >= 1.f)
                    return;
            }

            m_pos = m_finalPos;
        }

        void ScoreSprite::draw(sys::Renderer& rdr)
        {
            app::Sprite::draw(rdr);

            sys::Rect rect;
            if (m_numberDrawer.getSpriteSize(m_scale, rect.w, rect.h))
            {
                rect.x = static_cast<int>(std::round(m_pos.x - m_hotspot.x * rect.w + m_numberStampOffset.x * m_scale));
                rect.y = static_cast<int>(std::round(m_pos.y - m_hotspot.y * rect.h + m_numberStampOffset.y * m_scale));
                m_numberDrawer.drawSprite(rdr, rect);
            }
        }
    } // namespace score
} // namespace game

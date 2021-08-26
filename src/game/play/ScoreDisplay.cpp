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

#include "ScoreDisplay.h"

#include "../../sys/AudioMixer.h"
#include "../../sys/FrameInfo.h"

namespace game
{
    namespace play
    {
        void ScoreDisplay::resetScore()
        {
            m_uScore = 0;
            m_drawnScore = 0.f;
            m_numberDrawer.setNumber(0);
        }

        void ScoreDisplay::update(const sys::FrameInfo& frame)
        {
            if (m_drawnScore < m_uScore)
            {
                m_drawnScore += frame.getDuration() * m_scoreSpeed;
                if (m_drawnScore < m_uScore)
                    m_numberDrawer.setNumber(static_cast<unsigned int>(m_drawnScore));
                else
                {
                    m_drawnScore = static_cast<float>(m_uScore);
                    m_numberDrawer.setNumber(m_uScore);
                }

                if (m_pSuccessSample && (m_successSampleTracker.getStatus() == sys::AudioStatus::STOPPED))
                    m_pSuccessSample->play(0, &m_successSampleTracker);
            }
        }
    } // namespace play
} // namespace game

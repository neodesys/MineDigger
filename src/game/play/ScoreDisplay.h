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

#ifndef _SCOREDISPLAY_H_
#define	_SCOREDISPLAY_H_

#include "../../app/Sprite.h"
#include "../../app/NumberDrawer.h"
#include "../../sys/IAnimated.h"
#include "../../sys/AudioSample.h"

namespace game
{
	namespace play
	{
		class ScoreDisplay final : public app::Sprite, public sys::IAnimated
		{
		public:
			ScoreDisplay()
			{
				app::Sprite::setSpriteDrawer(&m_numberDrawer);
			}

			void setNumberStamp(const app::NumberStamp* pNumberStamp)
			{
				m_numberDrawer.setNumberStamp(pNumberStamp);
			}

			void setSuccessSample(sys::AudioSample* pSample)
			{
				m_pSuccessSample = pSample;
			}

			void setMinDigits(int minDigits)
			{
				m_numberDrawer.setMinDigits(minDigits);
			}

			void setColor(const sys::Color& color)
			{
				m_numberDrawer.setColor(color);
			}

			void setShadowColor(const sys::Color& color)
			{
				m_numberDrawer.setShadowColor(color);
			}

			void setShadowOffset(int x, int y)
			{
				m_numberDrawer.setShadowOffset(x, y);
			}

			void setScoreSpeed(float scoreSpeed) //in points per sec
			{
				m_scoreSpeed = (scoreSpeed > 0.f) ? scoreSpeed : 100.f;
			}

			void addScore(unsigned int uPoints)
			{
				m_uScore += uPoints;
			}

			unsigned int getScore() const
			{
				return m_uScore;
			}

			void resetScore();

			void update(const sys::FrameInfo& frame) override final;

		private:
			ScoreDisplay(const ScoreDisplay&) = delete;
			ScoreDisplay& operator=(const ScoreDisplay&) = delete;

			//Forbid access to base methods
			void setHotspot(const sys::Vec2&) = delete;
			void setSpriteDrawer(const app::ISpriteDrawer*) = delete;

			app::NumberDrawer m_numberDrawer;

			sys::AudioSample* m_pSuccessSample = nullptr;
			sys::AudioSample::Tracker m_successSampleTracker;

			float m_scoreSpeed = 100.f;
			float m_drawnScore = 0.f;

			unsigned int m_uScore = 0;
		};
	}
}

#endif //_SCOREDISPLAY_H_

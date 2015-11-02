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

#ifndef _SCORESPRITE_H_
#define	_SCORESPRITE_H_

#include "../../app/Sprite.h"
#include "../../app/NumberDrawer.h"
#include "../../app/TextureDrawer.h"
#include "../../sys/IAnimated.h"

namespace game
{
	namespace score
	{
		class ScoreSprite final : public app::Sprite, public sys::IAnimated
		{
		public:
			ScoreSprite()
			{
				app::Sprite::setSpriteDrawer(&m_textureDrawer);
			}

			void setTexture(const sys::Texture* pTexture, const sys::Rect* pClip = nullptr)
			{
				m_textureDrawer.setTexture(pTexture, pClip);
			}

			void setNumberStamp(const app::NumberStamp* pNumberStamp)
			{
				m_numberDrawer.setNumberStamp(pNumberStamp);
			}

			void setNumberStampOffset(const sys::Vec2& offset)
			{
				m_numberStampOffset = offset;
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

			void setFinalScore(unsigned int uFinalScore)
			{
				m_uFinalScore = uFinalScore;
			}

			void setFinalPos(const sys::Vec2& finalPos)
			{
				m_finalPos = finalPos;
			}

			void startAnim(unsigned long uAnimDuration); //in ms

			bool hasAnimFinished() const
			{
				return (m_pos == m_finalPos);
			}

			void update(const sys::FrameInfo& frame) override final;

			void draw(sys::Renderer& rdr) override final;

		private:
			ScoreSprite(const ScoreSprite&) = delete;
			ScoreSprite& operator=(const ScoreSprite&) = delete;

			//Forbid access to base methods
			void setHotspot(const sys::Vec2&) = delete;
			void setScale(float) = delete;
			void setSpriteDrawer(const app::ISpriteDrawer*) = delete;

			app::TextureDrawer m_textureDrawer;

			app::NumberDrawer m_numberDrawer;
			sys::Vec2 m_numberStampOffset;

			unsigned int m_uFinalScore = 0;
			float m_scoreSpeed = 0.f; //in units per sec

			sys::Vec2 m_finalPos;
			sys::Vec2 m_moveSpeed; //in pixels per sec

			float m_currentScore = 0.f;
		};
	}
}

#endif //_SCORESPRITE_H_

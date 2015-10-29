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

#ifndef _COUNTDOWN_H_
#define	_COUNTDOWN_H_

#include "../../app/Sprite.h"
#include "../../app/NumberDrawer.h"
#include "../../sys/IAnimated.h"

namespace game
{
	namespace play
	{
		class Countdown final : public app::Sprite, public sys::IAnimated
		{
		public:
			Countdown()
			{
				app::Sprite::setSpriteDrawer(&m_numberDrawer);
			}

			void setNumberPrintRes(const app::NumberPrintRes* pNumberPrintRes)
			{
				m_numberDrawer.setNumberPrintRes(pNumberPrintRes);
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

			void start(unsigned int uSec);

			unsigned int getCurrentValue() const
			{
				return m_uCurrentValue;
			}

			void update(const sys::FrameInfo& frame) override final;

		private:
			Countdown(const Countdown&) = delete;
			Countdown& operator=(const Countdown&) = delete;

			//Forbid access to base methods
			void setHotspot(const sys::Vec2&) = delete;
			void setSpriteDrawer(const app::ISpriteDrawer*) = delete;

			app::NumberDrawer m_numberDrawer;

			unsigned long m_uStartTimestamp = 0;
			unsigned int m_uDuration = 0;
			unsigned int m_uCurrentValue = 0;
		};
	}
}

#endif //_COUNTDOWN_H_

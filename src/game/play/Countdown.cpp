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

#include "Countdown.h"

#include "../../sys/FrameInfo.h"

namespace game
{
	namespace play
	{
		void Countdown::start(unsigned int uSec)
		{
			m_uCurrentValue = m_uDuration = uSec;
			m_uStartTimestamp = 0;

			int minDigits = 0;
			while (uSec)
			{
				minDigits++;
				uSec /= 10;
			}

			m_numberDrawer.setMinDigits(minDigits);
			m_numberDrawer.setDrawnValue(m_uCurrentValue);
		}

		void Countdown::update(const sys::FrameInfo& frame)
		{
			if (!m_uStartTimestamp)
			{
				m_uStartTimestamp = frame.getTimestamp();
				return;
			}

			unsigned long t = frame.getTimestamp();
			if (t <= m_uStartTimestamp)
				return;

			t = (t - m_uStartTimestamp) / 1000;

			if (t >= m_uDuration)
				m_uCurrentValue = 0;
			else
				m_uCurrentValue = m_uDuration - t;

			m_numberDrawer.setDrawnValue(m_uCurrentValue);
		}
	}
}

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

#include "FrameInfo.h"

namespace sys
{
	bool FrameInfo::update(unsigned long t)
	{
		if (!m_uTimestamp)
		{
			m_uTimestamp = t;
			return false;
		}

		if (t <= m_uTimestamp)
			return false;

		m_dt = static_cast<float>(t - m_uTimestamp) / 1000.f;
		m_uPrevTimestamp = m_uTimestamp;
		m_uTimestamp = t;

		if (m_dtPrev <= 0.f)
		{
			m_dtPrev = m_dt;
			return false;
		}

		m_dtVar = m_dt / m_dtPrev;
		m_dtPrev = m_dt;
		m_dt2 = m_dt * m_dt;

		return true;
	}

	void FrameInfo::reset()
	{
		m_uTimestamp = m_uPrevTimestamp = 0;
		m_dt = m_dtPrev = m_dt2 = 0.f;
		m_dtVar = 1.f;
	}
}

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

#include "PathSprite.h"

#include <cassert>

#include "../sys/FrameInfo.h"

namespace app
{
	void PathSprite::setAnimPath(const KeyFrame* pAnimPath, std::size_t uAnimPathLength)
	{
		if (pAnimPath && uAnimPathLength &&
			pAnimPath[uAnimPathLength - 1].uTimestamp)
		{
			m_pAnimPath = pAnimPath;
			m_uAnimPathLength = uAnimPathLength;
		}
		else
		{
			m_pAnimPath = nullptr;
			m_uAnimPathLength = 0;

			m_uAnimStartTimestamp = 0;
			m_bAnimRunning = false;
		}
	}

	bool PathSprite::startAnim()
	{
		if (!m_pAnimPath)
			return false;

		assert(m_uAnimPathLength);

		m_uAnimStartTimestamp = 0;
		m_bAnimRunning = true;

		return true;
	}

	void PathSprite::stopAnim()
	{
		m_bAnimRunning = false;
		m_uAnimStartTimestamp = 0;
	}

	void PathSprite::update(const sys::FrameInfo& frame)
	{
		if (!m_bAnimRunning || !m_pAnimPath)
			return;

		assert(m_uAnimPathLength);

		if (!m_uAnimStartTimestamp)
		{
			//Anim is just starting, we position sprite at the first key-frame
			m_uAnimStartTimestamp = frame.getTimestamp();

			const KeyFrame* pKey = m_pAnimPath;
			m_pos = pKey->pos;
			m_scale = pKey->scale;
			return;
		}

		unsigned long uAnimOffset = frame.getTimestamp();
		if (uAnimOffset <= m_uAnimStartTimestamp)
			return;

		//Compute animation offset timestamp according to animation ending
		//behavior
		uAnimOffset -= m_uAnimStartTimestamp;
		unsigned long uAnimDuration = m_pAnimPath[m_uAnimPathLength - 1].uTimestamp;
		assert(uAnimDuration);

		switch (m_animEndBehavior)
		{
		case AnimEndBehavior::STOP:
			if (uAnimOffset >= uAnimDuration)
			{
				const KeyFrame* pKey = m_pAnimPath + m_uAnimPathLength - 1;
				m_pos = pKey->pos;
				m_scale = pKey->scale;

				m_bAnimRunning = false;
				m_uAnimStartTimestamp = 0;
				return;
			}
			break;

		case AnimEndBehavior::REPEAT:
			uAnimOffset %= uAnimDuration + 1;
			break;

		case AnimEndBehavior::REVERSE:
			if ((uAnimOffset / (uAnimDuration + 1)) & 0x01)
				uAnimOffset = uAnimDuration - (uAnimOffset % (uAnimDuration + 1));
			else
				uAnimOffset %= uAnimDuration + 1;
			break;
		}

		//Find surrounding key-frames to interpolate sprite between.
		//We can do a binary search as key-frames array MUST be correctly
		//ordered by design.
		std::size_t uMin = 0;
		std::size_t uMax = m_uAnimPathLength - 1;
		while (uMin < uMax)
		{
			std::size_t uMid = (uMin + uMax) >> 1;
			if (uAnimOffset <= m_pAnimPath[uMid].uTimestamp)
				uMax = uMid;
			else
				uMin = uMid + 1;
		}

		const KeyFrame* pNextKey = m_pAnimPath + uMax;
		if (!uMax || (uAnimOffset == pNextKey->uTimestamp))
		{
			//We are before the first key-frame or exactly at a key-frame,
			//nothing to interpolate
			m_pos = pNextKey->pos;
			m_scale = pNextKey->scale;
		}
		else
		{
			//We are after the first key-frame and between two key-frames,
			//we need to interpolate
			const KeyFrame* pPrevKey = m_pAnimPath + uMax - 1;

			if ((pPrevKey->uTimestamp >= pNextKey->uTimestamp) ||
				(uAnimOffset < pPrevKey->uTimestamp))
			{
				//Key-frames should have distinct timestamps and be correctly
				//ordered but we still have to protect ourselves from bad
				//parameters...
				m_pos = pNextKey->pos;
				m_scale = pNextKey->scale;
				return;
			}

			float f = static_cast<float>(uAnimOffset - pPrevKey->uTimestamp) / (pNextKey->uTimestamp - pPrevKey->uTimestamp);
			switch (m_animInterpolation)
			{
			case AnimInterpolation::LINEAR:
				linearInterpolate(pPrevKey, f, pNextKey);
				break;

			case AnimInterpolation::CATMULL_ROM:
				catmullRomInterpolate(pPrevKey, f, pNextKey);
				break;
			}
		}
	}

	void PathSprite::linearInterpolate(const KeyFrame* pPrevKey, float f, const KeyFrame* pNextKey)
	{
		if (pPrevKey && pNextKey)
		{
			m_pos = pPrevKey->pos + (pNextKey->pos - pPrevKey->pos) * f;
			m_scale = pPrevKey->scale + (pNextKey->scale - pPrevKey->scale) * f;
		}
	}

	void PathSprite::catmullRomInterpolate(const KeyFrame* pPrevKey, float f, const KeyFrame* pNextKey)
	{
		if (pPrevKey && pNextKey)
		{
			const KeyFrame* pKey0 = pPrevKey;
			const KeyFrame* pKey3 = pNextKey;

			if (m_pAnimPath)
			{
				assert(m_uAnimPathLength);
				if (pKey0 > m_pAnimPath)
					pKey0--;

				if (pKey3 < m_pAnimPath + m_uAnimPathLength - 1)
					pKey3++;
			}

			//Catmull-Rom interpolation is a special Hermite interpolation with:
			//Ti = 0.5 * (Pi+1 - Pi-1)
			float ff = f * f;
			float g = -0.5f * f;

			float f0 = g * (ff + 1.f) + ff;
			float f1 = g * (5.f * f - 3.f * ff) + 1.f;
			float f2 = g * (3.f * ff - 4.f * f - 1.f);
			float f3 = g * (f - ff);

			m_pos = pKey0->pos * f0 + pPrevKey->pos * f1 + pNextKey->pos * f2 + pKey3->pos * f3;
			m_scale = pKey0->scale * f0 + pPrevKey->scale * f1 + pNextKey->scale * f2 + pKey3->scale * f3;
		}
	}
}

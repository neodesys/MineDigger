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

#ifndef _NUMBERDRAWER_H_
#define	_NUMBERDRAWER_H_

#include "ISpriteDrawer.h"
#include "../sys/sys.h"

namespace app
{
	class NumberPrintRes;

	class NumberDrawer : public ISpriteDrawer
	{
	public:
		void setNumberPrintRes(const NumberPrintRes* pNumberPrintRes)
		{
			m_pNumberPrintRes = pNumberPrintRes;
		}

		void setDrawnValue(unsigned int uDrawnValue)
		{
			m_uDrawnValue = uDrawnValue;
		}

		void setMinDigits(int minDigits)
		{
			m_minDigits = minDigits;
		}

		void setColor(const sys::Color& color)
		{
			m_color = color;
		}

		void setShadowColor(const sys::Color& color)
		{
			m_shadowColor = color;
		}

		void setShadowOffset(int x, int y)
		{
			m_shadowOffset[0] = x;
			m_shadowOffset[1] = y;
		}

		virtual bool getSpriteSize(float scale, int& w, int& h) const override;
		virtual void drawSprite(sys::Renderer& rdr, const sys::Rect& rect) const override;

	protected:
		const NumberPrintRes* m_pNumberPrintRes = nullptr;

		unsigned int m_uDrawnValue = 0;
		int m_minDigits = 0;

		sys::Color m_color;

		sys::Color m_shadowColor = {0, 0, 0, 0};
		int m_shadowOffset[2] = {};
	};
}

#endif //_NUMBERDRAWER_H_

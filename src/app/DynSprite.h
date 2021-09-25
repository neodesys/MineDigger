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

#ifndef _DYNSPRITE_H_
#define	_DYNSPRITE_H_

#include "Sprite.h"

namespace app
{
	class DynSprite : public Sprite
	{
	public:
		void setAcceleration(const sys::Vec2& v)
		{
			m_acceleration = v;
		}

		void setImpulse(const sys::Vec2& v)
		{
			m_moveVec = v;
		}

		//Update position using time corrected Verlet integration
		//P(i+1) - P(i) = (P(i) - P(i-1)) * dt(i)/dt(i-1) + A(i) * dt(i)²
		//with:
		//  dtCoeff = dt(i)/dt(i-1)
		//and:
		//  dt2 = dt(i)²
		virtual void updatePos(float dtCoeff, float dt2);

	protected:
		sys::Vec2 m_acceleration;
		sys::Vec2 m_moveVec;
	};
}

#endif //_DYNSPRITE_H_

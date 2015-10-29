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

#ifndef _VEC2_H_
#define	_VEC2_H_

#include "sys.h"

#include <cmath>

namespace sys
{
	class Vec2 final
	{
	public:
		float x = 0.f;
		float y = 0.f;

		static constexpr float EPSILON = 1e-6f;

		Vec2() = default;
		Vec2(float x, float y) : x(x), y(y) {}
		Vec2(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}

		Vec2& operator+=(const Vec2& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		Vec2& operator-=(const Vec2& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		Vec2 operator+(const Vec2& v) const
		{
			return Vec2(x + v.x, y + v.y);
		}

		Vec2 operator-(const Vec2& v) const
		{
			return Vec2(x - v.x, y - v.y);
		}

		Vec2& operator*=(float f)
		{
			x *= f;
			y *= f;
			return *this;
		}

		Vec2 operator*(float f) const
		{
			return Vec2(x * f, y * f);
		}

		float squareNorm() const
		{
			return x * x + y * y;
		}

		float norm() const
		{
			return std::sqrt(x * x + y * y);
		}

		Vec2& normalize()
		{
			float n = norm();
			if (n > 0.f)
				*this *= 1.f / n;
			else
				x = y = 0.f;

			return *this;
		}

		float distance(const Vec2& v) const
		{
			return (v - *this).norm();
		}

		float dot(const Vec2& v) const
		{
			return x * v.x + y * v.y;
		}

		bool isInside(const Rect& r) const
		{
			return ((x >= r.x) && (y >= r.y) &&
					(x < r.x + r.w) && (y < r.y + r.h));
		}

		bool operator==(const Vec2& v) const
		{
			return ((std::abs(v.x - x) <= EPSILON) &&
					(std::abs(v.y - y) <= EPSILON));
		}

		bool operator!=(const Vec2& v) const
		{
			return !(*this == v);
		}

		bool isNull() const
		{
			return ((std::abs(x) <= EPSILON) &&
					(std::abs(y) <= EPSILON));
		}
	};
}

#endif //_VEC2_H_

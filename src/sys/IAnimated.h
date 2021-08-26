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

#ifndef _IANIMATED_H_
#define _IANIMATED_H_

namespace sys
{
    class FrameInfo;

    class IAnimated
    {
      public:
        virtual ~IAnimated() = default;

        // It is guaranteed that frame parameter is always a valid FrameInfo:
        // - getPrevTimestamp() > 0
        // - getTimestamp() > getPrevTimestamp()
        // - getDuration() > 0.f
        // - getDurationVar() > 0.f
        // - getSquareDuration() > 0.f
        virtual void update(const FrameInfo& frame) = 0;
    };
} // namespace sys

#endif // _IANIMATED_H_

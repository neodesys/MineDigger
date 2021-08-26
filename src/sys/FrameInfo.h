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

#ifndef _FRAMEINFO_H_
#define _FRAMEINFO_H_

namespace sys
{
    class FrameInfo final
    {
      public:
        bool update(unsigned long t);
        void reset();

        unsigned long getTimestamp() const
        {
            return m_uTimestamp;
        }

        unsigned long getPrevTimestamp() const
        {
            return m_uPrevTimestamp;
        }

        float getDuration() const
        {
            return m_dt;
        }

        float getDurationVar() const
        {
            return m_dtVar;
        }

        float getSquareDuration() const
        {
            return m_dt2;
        }

      private:
        unsigned long m_uTimestamp = 0;     // in ms
        unsigned long m_uPrevTimestamp = 0; // in ms

        float m_dt = 0.f;     // in sec
        float m_dtPrev = 0.f; // in sec
        float m_dtVar = 1.f;  // = dt / dtPrev
        float m_dt2 = 0.f;    // = dt * dt
    };
} // namespace sys

#endif // _FRAMEINFO_H_

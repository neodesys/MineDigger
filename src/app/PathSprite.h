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

#ifndef _PATHSPRITE_H_
#define _PATHSPRITE_H_

#include "../sys/IAnimated.h"
#include "Sprite.h"

namespace app
{
    struct KeyFrame
    {
        unsigned long uTimestamp; // in ms
        sys::Vec2 pos;
        float scale;
    };

    enum struct AnimEndBehavior : unsigned char
    {
        STOP,
        REPEAT,
        REVERSE
    };

    enum struct AnimInterpolation : unsigned char
    {
        LINEAR,
        CATMULL_ROM
    };

    class PathSprite : public Sprite, public sys::IAnimated
    {
      public:
        // WARNING: animation key-frames are considered as a resource which can
        // be shared among different sprites, therefore pAnimPath array is NOT
        // copied. Thus, pAnimPath array lifetime MUST be superior to this
        // sprite lifetime.
        //
        // WARNING: animation key-frames MUST be ordered correctly with
        // DISTINCT and INCREASING uTimestamp timestamps. If the key-frames are
        // not correctly ordered or with identic timestamps, the sprite behavior
        // is undefined.
        void setAnimPath(const KeyFrame* pAnimPath, std::size_t uAnimPathLength);

        bool startAnim();
        void stopAnim();

        void setAnimEndBehavior(AnimEndBehavior behavior)
        {
            m_animEndBehavior = behavior;
        }

        void setAnimInterpolation(AnimInterpolation interpol)
        {
            m_animInterpolation = interpol;
        }

        virtual void update(const sys::FrameInfo& frame) override;

      protected:
        const KeyFrame* m_pAnimPath = nullptr;
        std::size_t m_uAnimPathLength = 0;

        unsigned long m_uAnimStartTimestamp = 0;
        bool m_bAnimRunning = false;

        AnimEndBehavior m_animEndBehavior = AnimEndBehavior::STOP;
        AnimInterpolation m_animInterpolation = AnimInterpolation::LINEAR;

        void linearInterpolate(const KeyFrame* pPrevKey, float f, const KeyFrame* pNextKey);
        void catmullRomInterpolate(const KeyFrame* pPrevKey, float f, const KeyFrame* pNextKey);
    };
} // namespace app

#endif // _PATHSPRITE_H_

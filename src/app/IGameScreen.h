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

#ifndef _IGAMESCREEN_H_
#define _IGAMESCREEN_H_

#include "../sys/IAnimated.h"
#include "../sys/IDrawable.h"
#include "../sys/IMouseListener.h"
#include "IResHolder.h"

namespace sys
{
    class AudioMixer;
}

namespace app
{
    class IGameScreen : public IResHolder, public sys::IMouseListener, public sys::IAnimated, public sys::IDrawable
    {
      public:
        virtual const char* getScreenName() = 0;

        // IGameScreen life-cycle
        //
        // This cycle is fully executed each time the IGame changes the active
        // game screen.
        //
        // 1- Resources initialization:
        //   - if getResState(nullptr) returns READY: go directly to screen
        //     main loop (no resources to load), else if it returns LOADING:
        //     loading screen is displayed and getResState(pEngine) is looped
        //     over until it returns READY
        //
        // 2- Screen main loop:
        //   - onGameScreenStart(mixer), all screen states and audio volumes
        //     should be reset here
        //   - main loop is running, processing at each iteration:
        //       - inputs (through IMouseListener methods)
        //       - update(frame)
        //       - draw(rdr)
        //   - onGameScreenEnd(mixer), screen states cleanup and
        //     stopping/pausing audio samples/music should be performed here
        //     if necessary
        //
        // 3- Resources cleaning:
        //   - cleanRes(false), must be transitive and also call all embedded
        //     IResHolders cleanRes(false) method
        //
        // Notice: screen resources are fully cleaned (cleanRes(true)) at the
        //        end of the embedding IGame life-cycle (see IGame)
        //
        // Notice: one-screen games should implement IGame and IGameScreen
        //        interfaces in the same object

        virtual void onGameScreenStart(sys::AudioMixer& mixer) = 0;
        virtual void onGameScreenEnd(sys::AudioMixer& mixer) = 0;
    };
} // namespace app

#endif // _IGAMESCREEN_H_

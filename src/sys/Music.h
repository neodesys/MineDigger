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

#ifndef _MUSIC_H_
#define _MUSIC_H_

#include "AudioMixer.h"

struct _Mix_Music;
typedef struct _Mix_Music Mix_Music;

namespace sys
{
    class GameEngine;

    class Music final
    {
      public:
        // Note: the active music is not necessarily in playing state. It may
        // also be in paused or stopped state if music has stopped consequently
        // to an error
        static Music* getActiveMusic()
        {
            return s_pActiveMusic;
        }

        static Music* loadMusic(const GameEngine& engine, const char* asset);
        ~Music();

        enum : int
        {
            FOREVER = -1
        };

        // There can be only one music playing at the same time, so calling
        // play() will stop any previously playing music
        bool play(int repeat = FOREVER);

        AudioStatus getStatus() const;

        void pause();
        void resume();
        void stop();

        // Volume is between 0.f and 1.f
        // If this Music is the active music, setVolume() will modify the global
        // music volume directly and getVolume() will return the global music
        // volume. If this Music is not the active music, setVolume() will do
        // nothing and getVolume() will return 0.f.
        void setVolume(float vol);
        float getVolume() const;

      private:
        Music(Mix_Music* pSDLMusic) : m_pSDLMusic(pSDLMusic)
        {
        }
        Music(const Music&) = delete;
        Music& operator=(const Music&) = delete;

        Mix_Music* const m_pSDLMusic;

        static const Logger s_log;

        static Music* s_pActiveMusic;
        friend void AudioMixer::stopMusic();
    };
} // namespace sys

#endif // _MUSIC_H_

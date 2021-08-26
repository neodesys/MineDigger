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

#include "Music.h"

#include <cassert>
#include <new>

#include <SDL2/SDL_mixer.h>

#include "GameEngine.h"
#include "ResLoader.h"

namespace sys
{
    const Logger Music::s_log("Music");

    Music* Music::s_pActiveMusic = nullptr;

    Music* Music::loadMusic(const GameEngine& engine, const char* asset)
    {
        Mix_Music* pSDLMusic = engine.getResLoader().loadMusic(asset);
        if (!pSDLMusic)
            return nullptr;

        Music* pMusic = new (std::nothrow) Music(pSDLMusic);
        if (!pMusic)
        {
            Mix_FreeMusic(pSDLMusic);
            s_log.critical("Out of memory");
            return nullptr;
        }

        s_log.info("Music loaded for \"%s\"", asset);
        return pMusic;
    }

    Music::~Music()
    {
        assert(m_pSDLMusic);
        Mix_FreeMusic(m_pSDLMusic);

        if (s_pActiveMusic == this)
            s_pActiveMusic = nullptr;
    }

    bool Music::play(int repeat)
    {
        assert(m_pSDLMusic);
        if (repeat >= FOREVER)
        {
            if (!Mix_PlayMusic(m_pSDLMusic, repeat))
            {
                s_pActiveMusic = this;
                return true;
            }

            s_log.warning("Cannot play music (%s)", Mix_GetError());
        }

        return false;
    }

    AudioStatus Music::getStatus() const
    {
        if (s_pActiveMusic == this)
        {
            if (Mix_PlayingMusic())
            {
                if (Mix_PausedMusic())
                    return AudioStatus::PAUSED;
                else
                    return AudioStatus::PLAYING;
            }

            s_pActiveMusic = nullptr;
        }

        return AudioStatus::STOPPED;
    }

    void Music::pause()
    {
        if (s_pActiveMusic == this)
            Mix_PauseMusic();
    }

    void Music::resume()
    {
        if (s_pActiveMusic == this)
            Mix_ResumeMusic();
    }

    void Music::stop()
    {
        if (s_pActiveMusic == this)
        {
            Mix_HaltMusic();
            s_pActiveMusic = nullptr;
        }
    }

    void Music::setVolume(float vol)
    {
        if (s_pActiveMusic == this)
        {
            int v = 0;
            if (vol > 0.f)
                v = static_cast<int>(MIX_MAX_VOLUME * vol);

            Mix_VolumeMusic(v);
        }
    }

    float Music::getVolume() const
    {
        if (s_pActiveMusic == this)
        {
            int vol = Mix_VolumeMusic(-1);
            if (vol > 0)
                return static_cast<float>(vol) / MIX_MAX_VOLUME;
        }

        return 0.f;
    }
} // namespace sys

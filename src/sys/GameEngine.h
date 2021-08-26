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

#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_

#include "Logger.h"

namespace sys
{
    class ResLoader;
    class AudioMixer;
    class Renderer;
    class IMouseListener;

    class GameEngine final
    {
      public:
        static GameEngine* createGameEngine(const char* wndName, int width, int height);
        ~GameEngine();

        ResLoader& getResLoader() const
        {
            return *m_pResLoader;
        }

        AudioMixer& getAudioMixer() const
        {
            return *m_pAudioMixer;
        }

        Renderer& getRenderer() const
        {
            return *m_pRenderer;
        }

        unsigned long getTicks() const; // in ms

        void setMouseListener(IMouseListener* pMouseListener)
        {
            m_pMouseListener = pMouseListener;
        }

        // Returns false on "quit" event
        bool processEvents();

      private:
        GameEngine(ResLoader* pResLoader, AudioMixer* pAudioMixer, Renderer* pRenderer)
            : m_pResLoader(pResLoader), m_pAudioMixer(pAudioMixer), m_pRenderer(pRenderer)
        {
        }
        GameEngine(const GameEngine&) = delete;
        GameEngine& operator=(const GameEngine&) = delete;

        static const Logger s_log;

        static bool initSDL2();
        static void shutSDL2();

        ResLoader* const m_pResLoader;
        AudioMixer* const m_pAudioMixer;
        Renderer* const m_pRenderer;

        bool m_bGameMustQuit = false;

        unsigned long m_uSystemPauseTimestamp = 0;
        unsigned long m_uSystemTicksOffset = 0;

        IMouseListener* m_pMouseListener = nullptr;
    };
} // namespace sys

#endif // _GAMEENGINE_H_

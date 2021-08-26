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

#include "GameEngine.h"

#include <cassert>
#include <new>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "AudioMixer.h"
#include "IMouseListener.h"
#include "Renderer.h"
#include "ResLoader.h"
#include "Vec2.h"

namespace
{
    const SDL_Keycode FULLSCREEN_KEY = SDLK_F11;

#ifdef __ANDROID__
    const SDL_Keycode QUIT_KEY = SDLK_AC_BACK;
#else
    const SDL_Keycode QUIT_KEY = SDLK_ESCAPE;
#endif // __ANDROID__

    bool s_bEngineAlreadyCreated = false;
} // namespace

namespace sys
{
    const Logger GameEngine::s_log("GameEngine");

    bool GameEngine::initSDL2()
    {
        // Init SDL core
        if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO))
        {
            s_log.critical("Cannot initialize SDL2 core (%s)", SDL_GetError());
            return false;
        }

        // Init SDL_image
        int flags = IMG_INIT_JPG | IMG_INIT_PNG;
        if ((IMG_Init(flags) & flags) != flags)
        {
            IMG_Quit(); // Image module may have initialized other plugins
            SDL_Quit(); // SDL_Quit() resets logs priority
            sys::Logger::setLogPriority(sys::Logger::getLogPriority());

            s_log.critical("Cannot initialize SDL2 image module (%s)", IMG_GetError());
            return false;
        }

        // Init SDL_mixer
        flags = MIX_INIT_OGG;
        if ((Mix_Init(flags) & flags) != flags)
        {
            while (Mix_Init(0))
                Mix_Quit(); // Mixer module may have initialized other plugins

            IMG_Quit();
            SDL_Quit(); // SDL_Quit() resets logs priority
            sys::Logger::setLogPriority(sys::Logger::getLogPriority());

            s_log.critical("Cannot initialize SDL2 mixer module (%s)", Mix_GetError());
            return false;
        }

        // Init SDL_ttf
        if (TTF_Init())
        {
            while (Mix_Init(0))
                Mix_Quit();

            IMG_Quit();
            SDL_Quit(); // SDL_Quit() resets logs priority
            sys::Logger::setLogPriority(sys::Logger::getLogPriority());

            s_log.critical("Cannot initialize SDL2 ttf module (%s)", TTF_GetError());
            return false;
        }

        s_log.info("SDL2 initialized");
        return true;
    }

    void GameEngine::shutSDL2()
    {
        TTF_Quit();

        while (Mix_Init(0))
            Mix_Quit();

        IMG_Quit();
        SDL_Quit(); // SDL_Quit() resets logs priority
        sys::Logger::setLogPriority(sys::Logger::getLogPriority());

        s_log.info("SDL2 shut");
    }

    GameEngine* GameEngine::createGameEngine(const char* wndName, int width, int height)
    {
        if (s_bEngineAlreadyCreated)
        {
            s_log.warning("Game engine already created");
            return nullptr;
        }

        if (!initSDL2())
            return nullptr;

        ResLoader* pResLoader = ResLoader::createResLoader();
        if (!pResLoader)
        {
            shutSDL2();
            return nullptr;
        }

        AudioMixer* pAudioMixer = AudioMixer::createAudioMixer();
        if (!pAudioMixer)
        {
            delete pResLoader;
            shutSDL2();
            return nullptr;
        }

        Renderer* pRenderer = Renderer::createRenderer(wndName, width, height);
        if (!pRenderer)
        {
            delete pAudioMixer;
            delete pResLoader;
            shutSDL2();
            return nullptr;
        }

        GameEngine* pEngine = new (std::nothrow) GameEngine(pResLoader, pAudioMixer, pRenderer);
        if (!pEngine)
        {
            delete pRenderer;
            delete pAudioMixer;
            delete pResLoader;
            shutSDL2();

            s_log.critical("Out of memory");
            return nullptr;
        }

        s_bEngineAlreadyCreated = true;
        s_log.info("Game engine created");
        return pEngine;
    }

    GameEngine::~GameEngine()
    {
        assert(m_pRenderer);
        assert(m_pAudioMixer);
        assert(m_pResLoader);

        delete m_pRenderer;
        delete m_pAudioMixer;
        delete m_pResLoader;

        s_log.info("Game engine destroyed");

        shutSDL2();
        s_bEngineAlreadyCreated = false;
    }

    unsigned long GameEngine::getTicks() const
    {
        unsigned long t = SDL_GetTicks();
        if (t > m_uSystemTicksOffset)
            return t - m_uSystemTicksOffset;
        else
            return 0;
    }

    bool GameEngine::processEvents()
    {
        assert(m_pRenderer);
        assert(m_pResLoader);
        assert(m_pAudioMixer);

        if (m_bGameMustQuit)
            return false;

        assert(!m_uSystemPauseTimestamp);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                m_bGameMustQuit = true;
                s_log.info("Quit requested");
                return false;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_MINIMIZED)
                {
                    // System must enter paused state because window has been
                    // minimized or application has been sent to background on
                    // portable devices (Android/iOS)
                    m_uSystemPauseTimestamp = event.window.timestamp;
                    if (!m_uSystemPauseTimestamp)
                    {
                        m_uSystemPauseTimestamp = SDL_GetTicks();
                        if (!m_uSystemPauseTimestamp)
                            m_uSystemPauseTimestamp = 1;
                    }

                    m_pAudioMixer->pauseAllSamples();
                    m_pAudioMixer->pauseMusic();
                    s_log.info("System paused");

                    // Block game main loop here until system is resumed or
                    // application has quit
                    while (m_uSystemPauseTimestamp)
                    {
                        SDL_Delay(100);

                        while (m_uSystemPauseTimestamp && SDL_PollEvent(&event))
                        {
                            switch (event.type)
                            {
                            case SDL_QUIT:
                                m_bGameMustQuit = true;
                                s_log.info("Quit requested");
                                return false;

                            case SDL_WINDOWEVENT:
                                if (event.window.event == SDL_WINDOWEVENT_RESTORED)
                                {
                                    unsigned long t = event.window.timestamp;
                                    if (!t)
                                        t = SDL_GetTicks();

                                    if (t > m_uSystemPauseTimestamp)
                                        m_uSystemTicksOffset += t - m_uSystemPauseTimestamp;

                                    m_uSystemPauseTimestamp = 0;
                                    m_pAudioMixer->resumeMusic();
                                    m_pAudioMixer->resumeAllSamples();
                                    s_log.info("System resumed");
                                }
                                break;
                            }
                        }
                    }
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case FULLSCREEN_KEY:
                    m_pRenderer->setFullscreen(!m_pRenderer->isFullscreen());
                    break;

                case QUIT_KEY:
                    m_bGameMustQuit = true;
#ifdef __ANDROID__
                    s_log.info("Quit requested from back button");
#else
                    s_log.info("Quit requested from ESC key");
#endif // __ANDROID__
                    return false;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (m_pMouseListener && (event.button.button == SDL_BUTTON_LEFT))
                {
                    const Vec2 pos = {event.button.x, event.button.y};
                    m_pMouseListener->onMouseButtonDown(pos);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (m_pMouseListener && (event.button.button == SDL_BUTTON_LEFT))
                {
                    const Vec2 pos = {event.button.x, event.button.y};
                    m_pMouseListener->onMouseButtonUp(pos);
                }
                break;

            case SDL_MOUSEMOTION:
                if (m_pMouseListener)
                {
                    const Vec2 pos = {event.motion.x, event.motion.y};
                    m_pMouseListener->onMouseMove(pos, (event.motion.state & SDL_BUTTON_LMASK));
                }
                break;
            }
        }

        return true;
    }
} // namespace sys

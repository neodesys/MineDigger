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

#include "ResLoader.h"
#include "SoundPlayer.h"
#include "Renderer.h"
#include "IMouseListener.h"

namespace
{
	const SDL_Keycode FULLSCREEN_KEY = SDLK_F11;

	const SDL_Keycode QUIT_KEY = SDLK_ESCAPE;

	bool s_bEngineAlreadyCreated = false;
}

namespace sys
{
	const Logger GameEngine::s_log("GameEngine");

	bool GameEngine::initSDL2()
	{
		//Init SDL core
		if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_EVENTS))
		{
			s_log.critical("Cannot initialize SDL2 core (%s)", SDL_GetError());
			return false;
		}

		//Init SDL_image
		int flags = IMG_INIT_JPG|IMG_INIT_PNG;
		if ((IMG_Init(flags) & flags) != flags)
		{
			IMG_Quit(); //Image module may have initialized other plugins
			SDL_Quit(); //SDL_Quit() resets logs priority
			sys::Logger::setLogPriority(sys::Logger::getLogPriority());

			s_log.critical("Cannot initialize SDL2 image module (%s)", IMG_GetError());
			return false;
		}

		//Init SDL_mixer
		flags = MIX_INIT_OGG;
		if ((Mix_Init(flags) & flags) != flags)
		{
			while (Mix_Init(0))
				Mix_Quit(); //Mixer module may have initialized other plugins

			IMG_Quit();
			SDL_Quit(); //SDL_Quit() resets logs priority
			sys::Logger::setLogPriority(sys::Logger::getLogPriority());

			s_log.critical("Cannot initialize SDL2 mixer module (%s)", Mix_GetError());
			return false;
		}

		//Init SDL_ttf
		if (TTF_Init())
		{
			while (Mix_Init(0))
				Mix_Quit();

			IMG_Quit();
			SDL_Quit(); //SDL_Quit() resets logs priority
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
		SDL_Quit(); //SDL_Quit() resets logs priority
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

		SoundPlayer* pSoundPlayer = SoundPlayer::createSoundPlayer();
		if (!pSoundPlayer)
		{
			delete pResLoader;
			shutSDL2();
			return nullptr;
		}

		Renderer* pRenderer = Renderer::createRenderer(wndName, width, height);
		if (!pRenderer)
		{
			delete pSoundPlayer;
			delete pResLoader;
			shutSDL2();
			return nullptr;
		}

		GameEngine* pEngine = new(std::nothrow) GameEngine(pResLoader, pSoundPlayer, pRenderer);
		if (!pEngine)
		{
			delete pRenderer;
			delete pSoundPlayer;
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
		delete m_pRenderer;
		delete m_pSoundPlayer;
		delete m_pResLoader;

		s_log.info("Game engine destroyed");

		shutSDL2();
		s_bEngineAlreadyCreated = false;
	}

	unsigned long GameEngine::getTicks() const
	{
		return SDL_GetTicks();
	}

	bool GameEngine::processEvents() const
	{
		assert(m_pRenderer);
		assert(m_pResLoader);
		assert(m_pSoundPlayer);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				s_log.info("Quit requested");
				return false;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case FULLSCREEN_KEY:
					m_pRenderer->setFullscreen(!m_pRenderer->isFullscreen());
					break;

				case QUIT_KEY:
					s_log.info("Quit requested from keyboard");
					return false;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (m_pMouseListener && (event.button.button == SDL_BUTTON_LEFT) && (event.button.clicks == 1))
					m_pMouseListener->onMouseButtonDown(event.button.x, event.button.y);
				break;

			case SDL_MOUSEBUTTONUP:
				if (m_pMouseListener && (event.button.button == SDL_BUTTON_LEFT) && (event.button.clicks == 1))
					m_pMouseListener->onMouseButtonUp(event.button.x, event.button.y);
				break;

			case SDL_MOUSEMOTION:
				if (m_pMouseListener)
					m_pMouseListener->onMouseMove(event.motion.x, event.motion.y, (event.motion.state & SDL_BUTTON_LMASK));
				break;
			}
		}

		return true;
	}
}

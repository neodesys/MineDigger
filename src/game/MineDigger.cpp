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

#include "MineDigger.h"

#include "../sys/Texture.h"

namespace
{
	#include "MineDigger.cfg"
}

namespace game
{
	MineDigger::MineDigger() :
		m_startScreen(*this),
		m_playScreen(*this, PLAYSCREEN_CONFIG),
		m_scoreScreen(*this) {}

	MineDigger::~MineDigger()
	{
		cleanRes(true);
		destroyLoadingDrawable();
	}

	const char* MineDigger::getGameName()
	{
		return GAME_NAME;
	}

	int MineDigger::getBoardWidth()
	{
		return BOARD_DIMENSIONS[0];
	}

	int MineDigger::getBoardHeight()
	{
		return BOARD_DIMENSIONS[1];
	}

	bool MineDigger::createLoadingDrawable(const sys::GameEngine& engine)
	{
		//TODO: build something smoother than a black loading screen
		return true;
	}

	sys::IDrawable* MineDigger::getLoadingDrawable()
	{
		return m_pLoadingDrawable;
	}

	void MineDigger::destroyLoadingDrawable()
	{
		delete m_pLoadingDrawable;
		m_pLoadingDrawable = nullptr;
	}

	void MineDigger::onGameStart()
	{
		m_pCurrentScreen = &m_startScreen;
	}

	void MineDigger::onGameEnd()
	{
		m_pCurrentScreen = nullptr;
	}

	app::IGameScreen* MineDigger::getCurrentGameScreen()
	{
		return m_pCurrentScreen;
	}

	app::ResState MineDigger::getResState(const sys::GameEngine* pEngine)
	{
		//For this particular game, we preload all screens resources while
		//loading game shared resources. The preloading is not compulsory as
		//app::run() automatically loads each screen resources when screen is
		//attached and before starting its main loop.

		app::ResState sharedResState = getSharedResState(pEngine);
		if (sharedResState != app::ResState::ERROR)
		{
			app::ResState startResState = m_startScreen.getResState(pEngine);
			if (startResState != app::ResState::ERROR)
			{
				app::ResState playResState = m_playScreen.getResState(pEngine);
				if (playResState != app::ResState::ERROR)
				{
					app::ResState scoreResState = m_scoreScreen.getResState(pEngine);
					if (scoreResState != app::ResState::ERROR)
					{
						if ((sharedResState == app::ResState::READY) &&
							(startResState == app::ResState::READY) &&
							(playResState == app::ResState::READY) &&
							(scoreResState == app::ResState::READY))
							return app::ResState::READY;
						else
							return app::ResState::LOADING;
					}
				}
			}
		}

		cleanRes(true);
		return app::ResState::ERROR;
	}

	void MineDigger::cleanRes(bool bForce)
	{
		m_scoreScreen.cleanRes(bForce);
		m_playScreen.cleanRes(bForce);
		m_startScreen.cleanRes(bForce);
		cleanSharedRes(bForce);
	}

	void MineDigger::switchToNextScreen()
	{
		if (m_pCurrentScreen == &m_startScreen)
			m_pCurrentScreen = &m_playScreen;
		else if (m_pCurrentScreen == &m_playScreen)
			m_pCurrentScreen = &m_scoreScreen;
		else
			m_pCurrentScreen = &m_startScreen;
	}

	app::ResState MineDigger::getSharedResState(const sys::GameEngine* pEngine)
	{
		if (m_pBackgroundTex)
			return app::ResState::READY;

		if (!pEngine)
			return app::ResState::LOADING;

		m_pBackgroundTex = sys::Texture::loadTexture(BACKGROUND_ASSET, *pEngine);
		if (m_pBackgroundTex)
			return app::ResState::READY;

		return app::ResState::ERROR;
	}

	void MineDigger::cleanSharedRes(bool bForce)
	{
		if (bForce)
		{
			delete m_pBackgroundTex;
			m_pBackgroundTex = nullptr;
		}
	}
}

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
#include "../sys/Font.h"
#include "../app/NumberStamp.h"

#ifdef __ANDROID__
#define WIDE_SCREEN 1
#endif //__ANDROID__

namespace
{
	#include "MineDigger.cfg"
}

namespace game
{
	MineDigger::MineDigger() :
		m_config(GAME_CONFIG),
		m_startScreen(*this),
		m_playScreen(*this),
		m_scoreScreen(*this) {}

	MineDigger::~MineDigger()
	{
		cleanRes(true);
		destroyLoadingDrawable();
	}

	const char* MineDigger::getGameName()
	{
		return m_config.gameName;
	}

	int MineDigger::getBoardWidth()
	{
		return m_config.boardDimensions[0];
	}

	int MineDigger::getBoardHeight()
	{
		return m_config.boardDimensions[1];
	}

	bool MineDigger::createLoadingDrawable(const sys::GameEngine& engine)
	{
		//For this particular game there is no need for a loading drawable as
		//resources loading is too fast
		return true;
	}

	sys::IDrawable* MineDigger::getLoadingDrawable()
	{
		return nullptr;
	}

	void MineDigger::destroyLoadingDrawable()
	{
		//Empty method
	}

	void MineDigger::onGameStart(sys::AudioMixer& mixer)
	{
		m_pCurrentScreen = &m_startScreen;
	}

	void MineDigger::onGameEnd(sys::AudioMixer& mixer)
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

		switch (getSharedResState(pEngine))
		{
		case app::ResState::LOADING:
			return app::ResState::LOADING;

		case app::ResState::ERROR:
			cleanRes(true);
			return app::ResState::ERROR;

		case app::ResState::READY:
			break;
		}

		switch (m_startScreen.getResState(pEngine))
		{
		case app::ResState::LOADING:
			return app::ResState::LOADING;

		case app::ResState::ERROR:
			cleanRes(true);
			return app::ResState::ERROR;

		case app::ResState::READY:
			break;
		}

		switch (m_playScreen.getResState(pEngine))
		{
		case app::ResState::LOADING:
			return app::ResState::LOADING;

		case app::ResState::ERROR:
			cleanRes(true);
			return app::ResState::ERROR;

		case app::ResState::READY:
			break;
		}

		switch (m_scoreScreen.getResState(pEngine))
		{
		case app::ResState::LOADING:
			return app::ResState::LOADING;

		case app::ResState::ERROR:
			cleanRes(true);
			return app::ResState::ERROR;

		case app::ResState::READY:
			break;
		}

		return app::ResState::READY;
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
		{
			m_scoreScreen.setFinalScore(m_playScreen.getFinalScore());
			m_pCurrentScreen = &m_scoreScreen;
		}
		else
			m_pCurrentScreen = &m_startScreen;
	}

	app::ResState MineDigger::getSharedResState(const sys::GameEngine* pEngine)
	{
		if (!m_pBackgroundTex)
		{
			if (pEngine)
			{
				m_pBackgroundTex = sys::Texture::loadTexture(*pEngine, m_config.backgroundAsset);
				if (!m_pBackgroundTex)
				{
					cleanSharedRes(true);
					return app::ResState::ERROR;
				}
			}

			return app::ResState::LOADING;
		}

		if (!m_pFont)
		{
			if (pEngine)
			{
				m_pFont = sys::Font::loadFont(*pEngine, m_config.fontAsset, m_config.fontPointSize);
				if (!m_pFont)
				{
					cleanSharedRes(true);
					return app::ResState::ERROR;
				}
			}

			return app::ResState::LOADING;
		}

		if (!m_pNumberStamp)
		{
			if (pEngine)
			{
				m_pNumberStamp = app::NumberStamp::createNumberStamp(*pEngine, *m_pFont);
				if (!m_pNumberStamp)
				{
					cleanSharedRes(true);
					return app::ResState::ERROR;
				}
			}

			return app::ResState::LOADING;
		}

		if (!m_pSuccessSample)
		{
			if (pEngine)
			{
				m_pSuccessSample = sys::AudioSample::loadSample(*pEngine, m_config.successSampleAsset);
				if (!m_pSuccessSample)
				{
					cleanSharedRes(true);
					return app::ResState::ERROR;
				}
			}

			return app::ResState::LOADING;
		}

		if (!m_pButtonSample)
		{
			if (pEngine)
			{
				m_pButtonSample = sys::AudioSample::loadSample(*pEngine, m_config.buttonSampleAsset);
				if (!m_pButtonSample)
				{
					cleanSharedRes(true);
					return app::ResState::ERROR;
				}
			}

			return app::ResState::LOADING;
		}

		return app::ResState::READY;
	}

	void MineDigger::cleanSharedRes(bool bForce)
	{
		if (bForce)
		{
			delete m_pButtonSample;
			m_pButtonSample = nullptr;

			delete m_pSuccessSample;
			m_pSuccessSample = nullptr;

			delete m_pNumberStamp;
			m_pNumberStamp = nullptr;

			delete m_pFont;
			m_pFont = nullptr;

			delete m_pBackgroundTex;
			m_pBackgroundTex = nullptr;
		}
	}
}

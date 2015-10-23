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

#include "../MineDigger.h"

#include "../../sys/Renderer.h"
#include "../../sys/Texture.h"

namespace
{
	const char SCREEN_NAME[] = "Play";
}

namespace game
{
	namespace play
	{
		PlayScreen::PlayScreen(MineDigger& game, const Config& config) :
			m_game(game), m_config(config),
			m_gemBoardView(m_pGemTexArray, config.gemBoardConfig) {}

		PlayScreen::~PlayScreen()
		{
			cleanRes(true);
		}

		const char* PlayScreen::getScreenName()
		{
			return SCREEN_NAME;
		}

		app::ResState PlayScreen::getResState(const sys::GameEngine* pEngine)
		{
			for (int i = 0; i < static_cast<int>(GemType::NB_GEM_TYPES); ++i)
			{
				if (!m_pGemTexArray[i])
				{
					if (!pEngine)
						return app::ResState::LOADING;

					m_pGemTexArray[i] = sys::Texture::loadTexture(m_config.gemResArray[i], *pEngine);
					if (!m_pGemTexArray[i])
					{
						cleanRes(true);
						return app::ResState::ERROR;
					}

					for (int j = i + 1; j < static_cast<int>(GemType::NB_GEM_TYPES); ++j)
					{
						if (!m_pGemTexArray[j])
							return app::ResState::LOADING;
					}

					return app::ResState::READY;
				}
			}

			return app::ResState::READY;
		}

		void PlayScreen::cleanRes(bool bForce)
		{
			if (bForce)
			{
				for (int i = 0; i < static_cast<int>(GemType::NB_GEM_TYPES); ++i)
				{
					delete m_pGemTexArray[i];
					m_pGemTexArray[i] = nullptr;
				}
			}
		}

		void PlayScreen::onGameScreenStart()
		{
			m_startTime = 0;
			m_lastFrameTime = 0;
			m_lastDeltaTime = 0.f;
			m_background.setTexture(m_game.getSharedBackgroundTex());
			m_gemBoardTop.setTexture(m_game.getSharedBackgroundTex(), m_config.gemBoardTopClip);
			m_gemBoardTop.setPos(m_config.gemBoardTopPos);
			m_gemBoardView.getModel().resetBoard(true);
		}

		void PlayScreen::onGameScreenEnd()
		{
			//As m_background and m_gemBoardView are private within PlayScreen,
			//they can't be drawn or updated outside of PlayScreen main loop.
			//Therefore, it is not compulsory to clean up anything here.
			//Correct pointers to resources will be set again on the next
			//PlayScreen::onGameScreenStart() call right before usage.
			//
			//Nevertheless, to enforce global sanity we reset all PlayScreen
			//variables to their default values in order to always guaranty
			//a neutral screen state outside of the screen main loop.

			m_gemBoardView.getModel().resetBoard(false);
			m_gemBoardTop.setPos({});
			m_gemBoardTop.setTexture(nullptr);
			m_background.setTexture(nullptr);
			m_startTime = 0;
			m_lastFrameTime = 0;
			m_lastDeltaTime = 0.f;
		}

		void PlayScreen::onMouseButtonDown(const sys::Vec2& pos)
		{
			m_gemBoardView.onMouseButtonDown(pos);
		}

		void PlayScreen::onMouseButtonUp(const sys::Vec2& pos)
		{
			m_gemBoardView.onMouseButtonUp(pos);
		}

		void PlayScreen::onMouseMove(const sys::Vec2& pos, bool bDragging)
		{
			m_gemBoardView.onMouseMove(pos, bDragging);
		}

		void PlayScreen::updateAnimations(unsigned long t)
		{
			if (!m_lastFrameTime)
			{
				m_lastFrameTime = t;
				return;
			}

			if (t <= m_lastFrameTime)
				return;

			const float dt = static_cast<float>(t - m_lastFrameTime) / 1000.f;
			m_lastFrameTime = t;

			if (m_lastDeltaTime <= 0)
			{
				m_lastDeltaTime = dt;
				return;
			}

			//If game has finished, go to next screen
			if (!m_startTime)
				m_startTime = t;
			else if ((t > m_startTime) && (t - m_startTime > m_config.uMaxPlayTime))
				m_game.switchToNextScreen();

			const float dtCoeff = dt / m_lastDeltaTime;
			const float dt2 = dt * dt;
			m_lastDeltaTime = dt;

			m_gemBoardView.updateSpritesPos(dtCoeff, dt2);
		}

		void PlayScreen::draw(sys::Renderer& rdr)
		{
			rdr.draw(m_background);
			m_gemBoardView.drawLayer(false, rdr);
			rdr.draw(m_gemBoardTop);
			m_gemBoardView.drawLayer(true, rdr);
		}
	}
}

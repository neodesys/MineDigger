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
		PlayScreen::PlayScreen(MineDigger& game) :
			m_game(game),
			m_config(game.getConfig().playScreenConfig),
			m_gemBoardView(*this)
		{
			m_gemBoardTop.setSpriteDrawer(&m_gemBoardTopDrawer);
		}

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
			for (int i = 0; i < GemType::NB_GEM_TYPES; ++i)
			{
				if (!m_pGemTexArray[i])
				{
					if (pEngine)
					{
						m_pGemTexArray[i] = sys::Texture::loadTexture(*pEngine, m_config.gemResArray[i]);
						if (!m_pGemTexArray[i])
						{
							cleanRes(true);
							return app::ResState::ERROR;
						}
					}

					return app::ResState::LOADING;
				}
			}

			return app::ResState::READY;
		}

		void PlayScreen::cleanRes(bool bForce)
		{
			if (bForce)
			{
				for (int i = 0; i < GemType::NB_GEM_TYPES; ++i)
				{
					delete m_pGemTexArray[i];
					m_pGemTexArray[i] = nullptr;
				}
			}
		}

		void PlayScreen::onGameScreenStart()
		{
			m_background.setTexture(m_game.getSharedBackgroundTex());

			m_gemBoardView.getModel().resetBoard(true);

			m_gemBoardTopDrawer.setTexture(m_game.getSharedBackgroundTex(), m_config.gemBoardTopClip);
			m_gemBoardTop.setPos(m_config.gemBoardTopPos);

			m_countdown.setPos(m_config.countdownPos);
			m_countdown.setScale(m_config.countdownScale);
			m_countdown.setNumberPrintRes(m_game.getSharedNumberPrintRes());
			m_countdown.setColor(m_config.countdownColor);
			m_countdown.setShadowColor(m_config.countdownShadowColor);
			m_countdown.setShadowOffset(m_config.countdownShadowOffset[0], m_config.countdownShadowOffset[1]);
			m_countdown.start(m_config.uCountdownDuration);

			m_scoreDisplay.setPos(m_config.scoreDisplayPos);
			m_scoreDisplay.setScale(m_config.scoreDisplayScale);
			m_scoreDisplay.setNumberPrintRes(m_game.getSharedNumberPrintRes());
			m_scoreDisplay.setMinDigits(m_config.scoreDisplayMinDigits);
			m_scoreDisplay.setColor(m_config.scoreDisplayColor);
			m_scoreDisplay.setShadowColor(m_config.scoreDisplayShadowColor);
			m_scoreDisplay.setShadowOffset(m_config.scoreDisplayShadowOffset[0], m_config.scoreDisplayShadowOffset[1]);
			m_scoreDisplay.setScoreSpeed(m_config.scoreDisplaySpeed);
			m_scoreDisplay.resetScore();
		}

		void PlayScreen::onGameScreenEnd()
		{
			//Empty method
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

		void PlayScreen::update(const sys::FrameInfo& frame)
		{
			m_gemBoardView.update(frame);
			m_countdown.update(frame);
			m_scoreDisplay.update(frame);

			if (!m_countdown.getCurrentValue())
				m_game.switchToNextScreen();
		}

		void PlayScreen::draw(sys::Renderer& rdr)
		{
			rdr.draw(m_background);
			m_gemBoardView.drawLayer(rdr, false);
			rdr.draw(m_gemBoardTop);
			rdr.draw(m_countdown);
			rdr.draw(m_scoreDisplay);
			m_gemBoardView.drawLayer(rdr, true);
		}

		const sys::Texture* PlayScreen::getGemTex(GemType type) const
		{
			if (type < GemType::NB_GEM_TYPES)
				return m_pGemTexArray[type];
			else
				return nullptr;
		}
	}
}

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

#ifndef _PLAYSCREEN_H_
#define	_PLAYSCREEN_H_

#include "../../app/IGameScreen.h"
#include "../../app/BackBoard.h"
#include "GemBoardView.h"
#include "Countdown.h"
#include "ScoreDisplay.h"

namespace game
{
	class MineDigger;

	namespace play
	{
		class PlayScreen final : public app::IGameScreen
		{
		public:
			struct Config
			{
				const char* gemResArray[GemType::NB_GEM_TYPES];

				GemBoardView::Config gemBoardConfig;

				sys::Rect gemBoardTopClip;
				sys::Vec2 gemBoardTopPos;

				sys::Vec2 countdownPos;
				float countdownScale;
				sys::Color countdownColor;
				sys::Color countdownShadowColor;
				int countdownShadowOffset[2];
				unsigned int uCountdownDuration; //in sec

				sys::Vec2 scoreDisplayPos;
				float scoreDisplayScale;
				int scoreDisplayMinDigits;
				sys::Color scoreDisplayColor;
				sys::Color scoreDisplayShadowColor;
				int scoreDisplayShadowOffset[2];
				float scoreDisplaySpeed; //in points per sec

				unsigned int uGemScorePoints;
			};

			PlayScreen(MineDigger& game);
			~PlayScreen() override final;

			const char* getScreenName() override final;

			app::ResState getResState(const sys::GameEngine* pEngine) override final;
			void cleanRes(bool bForce) override final;

			void onGameScreenStart() override final;
			void onGameScreenEnd() override final;

			void onMouseButtonDown(const sys::Vec2& pos) override final;
			void onMouseButtonUp(const sys::Vec2& pos) override final;
			void onMouseMove(const sys::Vec2& pos, bool bDragging) override final;

			void update(const sys::FrameInfo& frame) override final;

			void draw(sys::Renderer& rdr) override final;

			const Config& getConfig() const
			{
				return m_config;
			}

			const sys::Texture* getGemTex(GemType type) const;

			void addGemPoints()
			{
				m_scoreDisplay.addScore(m_config.uGemScorePoints);
			}

		private:
			PlayScreen(const PlayScreen&) = delete;
			PlayScreen& operator=(const PlayScreen&) = delete;

			MineDigger& m_game;
			const Config& m_config;

			app::BackBoard m_background;
			GemBoardView m_gemBoardView;
			app::TextureDrawer m_gemBoardTopDrawer;
			app::Sprite m_gemBoardTop;
			Countdown m_countdown;
			ScoreDisplay m_scoreDisplay;

			//Screen resources
			const sys::Texture* m_pGemTexArray[GemType::NB_GEM_TYPES] = {};
		};
	}
}

#endif //_PLAYSCREEN_H_

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
#include "SparkSprite.h"
#include "Countdown.h"
#include "ScoreDisplay.h"

namespace sys
{
	class Music;
}

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
				const char* gemAssetArray[GemType::NB_GEM_TYPES];
				const char* sparkAsset;

				const char* playMusicAsset;

				const char* swapSampleAsset;
				const char* cancelSampleAsset;
				const char* countdownSampleAsset;

				GemBoardView::Config gemBoardConfig;

				sys::Rect gemBoardTopClip;
				sys::Vec2 gemBoardTopPos;

				const app::KeyFrame* pSparkAnimPath;
				std::size_t uSparkAnimPathLength;
				unsigned int uSparkAnimSubImagesCount;
				unsigned int uSparkAnimSubImagesStride;
				float uSparkAnimSubImagesSpeed; //in images per sec

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

			void onGameScreenStart(sys::AudioMixer& mixer) override final;
			void onGameScreenEnd(sys::AudioMixer& mixer) override final;

			void onMouseButtonDown(const sys::Vec2& pos) override final;
			void onMouseButtonUp(const sys::Vec2& pos) override final;
			void onMouseMove(const sys::Vec2& pos, bool bDragging) override final;

			void update(const sys::FrameInfo& frame) override final;

			void draw(sys::Renderer& rdr) override final;

			void addGemPoints()
			{
				m_scoreDisplay.addScore(m_config.uGemScorePoints);
			}

			unsigned int getFinalScore() const
			{
				return m_scoreDisplay.getScore();
			}

			const Config& getConfig() const
			{
				return m_config;
			}

			const sys::Texture* getGemTex(GemType type) const;

			sys::AudioSample* getSwapSample() const
			{
				return m_pSwapSample;
			}

			sys::AudioSample* getCancelSample() const
			{
				return m_pCancelSample;
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
			SparkSprite m_sparkSprite;
			Countdown m_countdown;
			ScoreDisplay m_scoreDisplay;

			//Screen resources
			const sys::Texture* m_pGemTexArray[GemType::NB_GEM_TYPES] = {};
			sys::Texture* m_pSparkTex = nullptr;
			sys::Music* m_pPlayMusic = nullptr;
			sys::AudioSample* m_pSwapSample = nullptr;
			sys::AudioSample* m_pCancelSample = nullptr;
			sys::AudioSample* m_pCountdownSample = nullptr;
		};
	}
}

#endif //_PLAYSCREEN_H_

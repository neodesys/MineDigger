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

#ifndef _SCORESCREEN_H_
#define	_SCORESCREEN_H_

#include "../../app/IGameScreen.h"
#include "../../app/BackBoard.h"
#include "../../sys/AudioSample.h"
#include "ScoreSprite.h"
#include "BackButton.h"

namespace sys
{
	class Music;
}

namespace game
{
	class MineDigger;

	namespace score
	{
		class ScoreScreen final : public app::IGameScreen
		{
		public:
			struct Config
			{
				const char* scoreBackAsset;
				const char* scoreAsset;

				const char* scoreMusicAsset;
				const char* exploseSampleAsset;
				const char* victorySampleAsset;

				sys::Color explosionColor;
				float explosionSpeedinMs; //in color units per ms

				sys::Rect scoreClip;
				sys::Vec2 scoreNumberStampOffset;
				int scoreMinDigits;
				sys::Color scoreColor;
				sys::Color scoreShadowColor;
				int scoreShadowOffset[2];
				sys::Vec2 scoreInitialPos;
				sys::Vec2 scoreFinalPos;
				unsigned long uScoreAnimDuration; //in ms

				sys::Vec2 backButtonPos;
				sys::Rect backButtonClip;
				sys::Rect backButtonOverlayClip;
				unsigned int uBackButtonOverlayImgCount;
				unsigned int uBackButtonOverlayImgStride;

				sys::Rect backButtonRect;
			};

			ScoreScreen(MineDigger& game);
			~ScoreScreen() override final;

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

			void setFinalScore(unsigned int uFinalScore)
			{
				m_scoreSprite.setFinalScore(uFinalScore);
			}

			void playButtonSample() const;
			void playAgain();

		private:
			ScoreScreen(const ScoreScreen&) = delete;
			ScoreScreen& operator=(const ScoreScreen&) = delete;

			MineDigger& m_game;
			const Config& m_config;

			enum struct AnimState : unsigned char
			{
				EXPLODE,
				WAITING_SCORE,
				BACK_BUTTON
			};

			AnimState m_animState = AnimState::EXPLODE;
			unsigned long m_uAnimStartTimestamp = 0;

			sys::Color m_explosionColor;
			float m_explosionSpeedinMs = 1.f; //in color units per ms

			app::BackBoard m_background;
			ScoreSprite m_scoreSprite;
			BackButton m_backButton;

			//Screen resources
			const sys::Texture* m_pScoreBackTex = nullptr;
			const sys::Texture* m_pScoreTex = nullptr;
			sys::Music* m_pScoreMusic = nullptr;
			sys::AudioSample* m_pExploseSample = nullptr;
			sys::AudioSample* m_pVictorySample = nullptr;
			sys::AudioSample::Tracker m_sampleTracker;
		};
	}
}

#endif //_SCORESCREEN_H_

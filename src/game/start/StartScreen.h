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

#ifndef _STARTSCREEN_H_
#define	_STARTSCREEN_H_

#include "../../app/IGameScreen.h"
#include "../../app/BackBoard.h"
#include "StartBoard.h"

namespace sys
{
	class Music;
	class AudioSample;
}

namespace game
{
	class MineDigger;

	namespace start
	{
		class StartScreen final : public app::IGameScreen
		{
		public:
			struct Config
			{
				const char* diggerAsset;
				const char* startBoardAsset;

				const char* startMusicAsset;
				const char* startSampleAsset;

				sys::Rect startBoardTopClip;
				sys::Vec2 startBoardTopPos;

				sys::Vec2 diggerPos;
				sys::Vec2 diggerOutImpulse;
				sys::Vec2 diggerOutAcceleration;

				sys::Vec2 startBoardPos;
				sys::Vec2 startBoardOutImpulse;
				sys::Vec2 startBoardOutAcceleration;

				sys::Rect startBoardClip;
				sys::Rect startBoardOverlayClip;
				unsigned int uStartBoardOverlayImgCount;
				unsigned int uStartBoardOverlayImgStride;
				sys::Vec2 startBoardOverlayOffset;

				sys::Rect startButtonRect;
			};

			StartScreen(MineDigger& game);
			~StartScreen() override final;

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

			void playButtonSample() const;
			void launchGame();

		private:
			StartScreen(const StartScreen&) = delete;
			StartScreen& operator=(const StartScreen&) = delete;

			MineDigger& m_game;
			const Config& m_config;

			bool m_bLaunchingGame = false;

			app::BackBoard m_background;
			app::TextureDrawer m_startBoardTopDrawer;
			app::Sprite m_startBoardTop;
			StartSprite m_digger;
			StartBoard m_startBoard;

			//Screen resources
			const sys::Texture* m_pDiggerTex = nullptr;
			const sys::Texture* m_pStartBoardTex = nullptr;
			sys::Music* m_pStartMusic = nullptr;
			sys::AudioSample* m_pStartSample = nullptr;
		};
	}
}

#endif //_STARTSCREEN_H_

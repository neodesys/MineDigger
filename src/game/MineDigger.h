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

#ifndef _MINEDIGGER_H_
#define	_MINEDIGGER_H_

#include "../app/IGame.h"
#include "start/StartScreen.h"
#include "play/PlayScreen.h"
#include "score/ScoreScreen.h"

namespace sys
{
	class Font;
}

namespace game
{
	class MineDigger final : public app::IGame
	{
	public:
		struct Config
		{
			const char* gameName;
			int boardDimensions[2];

			const char* backgroundAsset;

			const char* fontAsset;
			int fontPointSize;

			play::PlayScreen::Config playScreenConfig;
		};

		MineDigger();
		~MineDigger() override final;

		const char* getGameName() override final;
		int getBoardWidth() override final;
		int getBoardHeight() override final;

		bool createLoadingDrawable(const sys::GameEngine& engine) override final;
		sys::IDrawable* getLoadingDrawable() override final;
		void destroyLoadingDrawable() override final;

		void onGameStart() override final;
		void onGameEnd() override final;

		app::IGameScreen* getCurrentGameScreen() override final;

		app::ResState getResState(const sys::GameEngine* pEngine) override final;
		void cleanRes(bool bForce) override final;

		const Config& getConfig() const
		{
			return m_config;
		}

		void switchToNextScreen();

		const sys::Texture* getSharedBackgroundTex() const
		{
			return m_pBackgroundTex;
		}

		const app::NumberPrintRes* getSharedNumberPrintRes() const
		{
			return m_pNumberPrintRes;
		}

	private:
		MineDigger(const MineDigger&) = delete;
		MineDigger& operator=(const MineDigger&) = delete;

		const Config& m_config;

		start::StartScreen m_startScreen;
		play::PlayScreen m_playScreen;
		score::ScoreScreen m_scoreScreen;

		sys::IDrawable* m_pLoadingDrawable = nullptr;
		app::IGameScreen* m_pCurrentScreen = nullptr;

		app::ResState getSharedResState(const sys::GameEngine* pEngine);
		void cleanSharedRes(bool bForce);

		//Shared resources
		const sys::Texture* m_pBackgroundTex = nullptr;
		sys::Font* m_pFont = nullptr;
		const app::NumberPrintRes* m_pNumberPrintRes = nullptr;
	};
}

#endif //_MINEDIGGER_H_

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
#include "StartScreen.h"
#include "PlayScreen.h"
#include "ScoreScreen.h"

namespace game
{
	class MineDigger final : public app::IGame
	{
	public:
		MineDigger() : m_startScreen(*this), m_playScreen(*this), m_scoreScreen(*this) {}
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

		void switchToNextScreen();

	private:
		MineDigger(const MineDigger&) = delete;
		MineDigger& operator=(const MineDigger&) = delete;

		StartScreen m_startScreen;
		PlayScreen m_playScreen;
		ScoreScreen m_scoreScreen;

		sys::IDrawable* m_pLoadingDrawable = nullptr;
		app::IGameScreen* m_pCurrentScreen = nullptr;

		app::ResState getSharedResState(const sys::GameEngine* pEngine);
		void cleanSharedRes(bool bForce);
	};
}

#endif //_MINEDIGGER_H_

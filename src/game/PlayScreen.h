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

#include "../app/IGameScreen.h"

namespace game
{
	class MineDigger;

	class PlayScreen final : public app::IGameScreen
	{
	public:
		PlayScreen(MineDigger& game) : m_game(game) {}
		~PlayScreen() override final;

		const char* getScreenName() override final;

		app::ResState getResState(const sys::GameEngine* pEngine) override final;
		void cleanRes(bool bForce) override final;

		void onGameScreenStart() override final;
		void onGameScreenEnd() override final;

		void onMouseButtonDown(int x, int y) override final;
		void onMouseButtonUp(int x, int y) override final;
		void onMouseMove(int x, int y, bool bDragging) override final;

		void updateAnimations(unsigned long t) override final;

		void draw(sys::Renderer& rdr) override final;

	private:
		PlayScreen(const PlayScreen&) = delete;
		PlayScreen& operator=(const PlayScreen&) = delete;

		MineDigger& m_game;
	};
}

#endif //_PLAYSCREEN_H_

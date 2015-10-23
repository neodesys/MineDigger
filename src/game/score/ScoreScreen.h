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

namespace game
{
	class MineDigger;

	namespace score
	{
		class ScoreScreen final : public app::IGameScreen
		{
		public:
			ScoreScreen(MineDigger& game) : m_game(game) {}
			~ScoreScreen() override final;

			const char* getScreenName() override final;

			app::ResState getResState(const sys::GameEngine* pEngine) override final;
			void cleanRes(bool bForce) override final;

			void onGameScreenStart() override final;
			void onGameScreenEnd() override final;

			void onMouseButtonDown(const sys::Vec2& pos) override final;
			void onMouseButtonUp(const sys::Vec2& pos) override final;
			void onMouseMove(const sys::Vec2& pos, bool bDragging) override final;

			void updateAnimations(unsigned long t) override final;

			void draw(sys::Renderer& rdr) override final;

		private:
			ScoreScreen(const ScoreScreen&) = delete;
			ScoreScreen& operator=(const ScoreScreen&) = delete;

			MineDigger& m_game;
			app::BackBoard m_background;
		};
	}
}

#endif //_SCORESCREEN_H_

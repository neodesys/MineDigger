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

#include "MineDigger.h"

#include "../sys/Renderer.h"

namespace
{
	const char SCREEN_NAME[] = "Score";
}

namespace game
{
	ScoreScreen::~ScoreScreen()
	{
		cleanRes(true);
	}

	const char* ScoreScreen::getScreenName()
	{
		return SCREEN_NAME;
	}

	app::ResState ScoreScreen::getResState(const sys::GameEngine* pEngine)
	{
		//TODO
		return app::ResState::READY;
	}

	void ScoreScreen::cleanRes(bool bForce)
	{
		//TODO
	}

	void ScoreScreen::onGameScreenStart()
	{
		//TODO
	}

	void ScoreScreen::onGameScreenEnd()
	{
		//TODO
	}

	void ScoreScreen::onMouseButtonDown(int x, int y)
	{
		//TODO
		m_game.switchToNextScreen();
	}

	void ScoreScreen::onMouseButtonUp(int x, int y)
	{
		//TODO
	}

	void ScoreScreen::onMouseMove(int x, int y, bool bDragging)
	{
		//TODO
	}

	void ScoreScreen::updateAnimations(unsigned long t)
	{
		//TODO
	}

	void ScoreScreen::draw(sys::Renderer& rdr)
	{
		//TODO
		rdr.clearBackground(0, 0, 255, 255);
	}
}

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

#ifndef _IGAME_H_
#define	_IGAME_H_

#include "IResHolder.h"

namespace sys
{
	class IDrawable;
	class AudioMixer;
}

namespace app
{
	class IGameScreen;

	class IGame : public IResHolder
	{
	public:
		virtual const char* getGameName() = 0;
		virtual int getBoardWidth() = 0;
		virtual int getBoardHeight() = 0;

		//IGame life-cycle:
		//
		//1- Resources initialization:
		//   - createLoadingDrawable(engine), must be simple and return quickly
		//   - getLoadingDrawable() called only once, the returned IDrawable
		//     pointer is stored for further use whenever a loading screen
		//     needs to be displayed. If returning nullptr (no specific loading
		//     screen), the default blank screen (a black screen) will be used
		//   - if getResState(nullptr) returns READY: go directly to game main
		//     loop (no resources to load), else if it returns LOADING: loading
		//     screen is displayed and getResState(pEngine) is looped over
		//     until it returns READY
		//
		//2- Game main loop:
		//   - onGameStart(mixer), all game states and audio volumes should be
		//     reset here
		//   - main loop is running, calling getCurrentGameScreen() at the
		//     beginning of each iteration, switching active game screen on
		//     change (see IGameScreen)
		//   - onGameEnd(mixer), game states cleanup and stopping/pausing audio
		//     samples/music should be performed here if necessary
		//
		//3- Resources cleaning:
		//   - cleanRes(true), must be transitive and also call all embedded
		//     game screens and IResHolders cleanRes(true) method
		//   - destroyLoadingDrawable()

		virtual bool createLoadingDrawable(const sys::GameEngine& engine) = 0;
		virtual sys::IDrawable* getLoadingDrawable() = 0;
		virtual void destroyLoadingDrawable() = 0;

		virtual void onGameStart(sys::AudioMixer& mixer) = 0;
		virtual void onGameEnd(sys::AudioMixer& mixer) = 0;

		virtual IGameScreen* getCurrentGameScreen() = 0;
	};
}

#endif //_IGAME_H_

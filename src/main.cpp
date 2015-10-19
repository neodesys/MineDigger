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

#include <SDL2/SDL.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif //_WIN32

#include "Logger.h"

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif //_WIN32
{
	Logger::setLogPriority(LogLevel::INFO);
	const Logger log("main");

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER))
	{
		log.critical("Cannot initialize SDL2 (%s)", SDL_GetError());
		return -1;
	}

	SDL_Window* pWnd = SDL_CreateWindow("MineDigger", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 755, 600, SDL_WINDOW_SHOWN);
	if (!pWnd)
	{
		log.critical("Cannot create game window (%s)", SDL_GetError());
		SDL_Quit();
		return -2;
	}

	bool bQuit = false;
	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				bQuit = true;
				break;

			case SDL_KEYUP:
				//TODO
				break;
			}
		}

		if (bQuit)
			break;

		//TODO: implement game main loop
	}

	SDL_DestroyWindow(pWnd);
	SDL_Quit();
	return 0;
}

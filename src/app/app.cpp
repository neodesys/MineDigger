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

#include "app.h"

#include "IGame.h"
#include "IGameScreen.h"
#include "../sys/GameEngine.h"
#include "../sys/Renderer.h"

namespace app
{
	RetCode run(IGame& game)
	{
		RetCode ret = RetCode::SUCCESS;

		//Init game engine
		const char* const gameName = game.getGameName();
		sys::GameEngine* const pEngine = sys::GameEngine::createGameEngine(gameName, game.getBoardWidth(), game.getBoardHeight());
		if (pEngine)
		{
			const sys::Logger log("app::run");
			log.info("Game application \"%s\" started", gameName);

			//Init loading screen
			if (game.createLoadingDrawable(*pEngine))
			{
				//Init game resources (if needed)
				ResState resState = game.getResState(nullptr);
				if (resState != ResState::ERROR)
				{
					sys::IDrawable* const pLoader = game.getLoadingDrawable();
					bool bQuit = false;
					bool bGameStarted = false;

					//Load game resources progressively while showing loading
					//screen
					if (resState != ResState::READY)
					{
						log.info("Loading game resources...");

						while (!bQuit)
						{
							if (!pEngine->processEvents())
								bQuit = true;
							else
							{
								pEngine->getRenderer().renderFrame(pLoader);

								resState = game.getResState(pEngine);
								if (resState == ResState::ERROR)
								{
									bQuit = true;
									log.critical("Cannot initialize \"%s\" game resources", gameName);
									ret = RetCode::ERR_GAME_RES_LOADING;
								}
								else if (resState == ResState::READY)
								{
									game.onGameStart();
									bGameStarted = true;
									break;
								}
							}
						}
					}
					else
					{
						game.onGameStart();
						bGameStarted = true;
					}

					//Manage game life-cycle
					IGameScreen* pCurrentScreen = nullptr;
					while (!bQuit)
					{
						IGameScreen* pGameScreen = game.getCurrentGameScreen();
						if (pGameScreen != pCurrentScreen)
						{
							//Switch game screen
							if (pCurrentScreen)
							{
								//Detach previous game screen
								//Previous game screen is free to decide itself
								//if it wants to keep its resources or not as
								//it may be re-used later depending on the
								//specific game logic and structure
								pEngine->setMouseListener(nullptr);
								pCurrentScreen->onGameScreenEnd();
								pCurrentScreen->cleanRes(false);
								pCurrentScreen = nullptr;
							}

							//Attach new game screen
							if (pGameScreen)
							{
								const char* const screenName = pGameScreen->getScreenName();
								log.info("Game has switched to \"%s::%s\" screen", gameName, screenName);

								//Init screen resources (if needed)
								resState = pGameScreen->getResState(nullptr);
								if (resState != ResState::ERROR)
								{
									//Load game screen resources progressively
									//while showing loading screen
									if (resState != ResState::READY)
									{
										log.info("Loading screen resources...");

										while (!bQuit)
										{
											if (!pEngine->processEvents())
												bQuit = true;
											else
											{
												pEngine->getRenderer().renderFrame(pLoader);

												resState = pGameScreen->getResState(pEngine);
												if (resState == ResState::ERROR)
												{
													bQuit = true;
													log.critical("Cannot initialize \"%s::%s\" screen resources", gameName, screenName);
													ret = RetCode::ERR_SCREEN_RES_LOADING;
												}
												else if (resState == ResState::READY)
													break;
											}
										}
									}

									if (resState == ResState::READY)
									{
										pCurrentScreen = pGameScreen;
										pCurrentScreen->onGameScreenStart();
										pEngine->setMouseListener(pCurrentScreen);
										log.info("Starting screen main loop...");
									}
								}
								else
								{
									bQuit = true;
									log.critical("Cannot initialize \"%s::%s\" screen resources", gameName, screenName);
									ret = RetCode::ERR_SCREEN_RES_LOADING;
								}
							}
							else
								log.warning("Game \"%s\" has switched to a null game screen", gameName);
						}
						else
						{
							//Game main loop step execution
							if (!pEngine->processEvents())
								bQuit = true;
							else
							{
								if (pCurrentScreen)
								{
									pCurrentScreen->updateAnimations(pEngine->getTicks());
									pEngine->getRenderer().renderFrame(pCurrentScreen);
								}
								else
									pEngine->getRenderer().renderFrame(pLoader);
							}
						}
					}

					//Detach last used game screen
					if (pCurrentScreen)
					{
						pEngine->setMouseListener(nullptr);
						pCurrentScreen->onGameScreenEnd();
						pCurrentScreen->cleanRes(true);
					}

					if (bGameStarted)
						game.onGameEnd();
				}
				else
				{
					log.critical("Cannot initialize \"%s\" game resources", gameName);
					ret = RetCode::ERR_GAME_RES_LOADING;
				}

				//Clean all game resources and loading screen
				game.cleanRes(true);
				game.destroyLoadingDrawable();
			}
			else
			{
				log.critical("Cannot initialize \"%s\" loading screen", gameName);
				ret = RetCode::ERR_NO_LOADING_SCREEN;
			}

			log.info("Game application \"%s\" finished", gameName);

			//Shut game engine
			delete pEngine;
		}
		else
			ret = RetCode::ERR_NO_ENGINE;

		return ret;
	}
}

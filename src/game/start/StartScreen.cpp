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

#include "../MineDigger.h"

#include "../../sys/Music.h"
#include "../../sys/Renderer.h"
#include "../../sys/Texture.h"

namespace
{
    const char SCREEN_NAME[] = "Start";
}

namespace game
{
    namespace start
    {
        StartScreen::StartScreen(MineDigger& game)
            : m_game(game), m_config(game.getConfig().startScreenConfig), m_startBoard(*this)
        {
            m_startBoardTop.setSpriteDrawer(&m_startBoardTopDrawer);
        }

        StartScreen::~StartScreen()
        {
            cleanRes(true);
        }

        const char* StartScreen::getScreenName()
        {
            return SCREEN_NAME;
        }

        app::ResState StartScreen::getResState(const sys::GameEngine* pEngine)
        {
            if (!m_pDiggerTex)
            {
                if (pEngine)
                {
                    m_pDiggerTex = sys::Texture::loadTexture(*pEngine, m_config.diggerAsset);
                    if (!m_pDiggerTex)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            if (!m_pStartBoardTex)
            {
                if (pEngine)
                {
                    m_pStartBoardTex = sys::Texture::loadTexture(*pEngine, m_config.startBoardAsset);
                    if (!m_pStartBoardTex)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            if (!m_pStartMusic)
            {
                if (pEngine)
                {
                    m_pStartMusic = sys::Music::loadMusic(*pEngine, m_config.startMusicAsset);
                    if (!m_pStartMusic)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            if (!m_pStartSample)
            {
                if (pEngine)
                {
                    m_pStartSample = sys::AudioSample::loadSample(*pEngine, m_config.startSampleAsset);
                    if (!m_pStartSample)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            return app::ResState::READY;
        }

        void StartScreen::cleanRes(bool bForce)
        {
            if (bForce)
            {
                delete m_pStartSample;
                m_pStartSample = nullptr;

                delete m_pStartMusic;
                m_pStartMusic = nullptr;

                delete m_pStartBoardTex;
                m_pStartBoardTex = nullptr;

                delete m_pDiggerTex;
                m_pDiggerTex = nullptr;
            }
        }

        void StartScreen::onGameScreenStart(sys::AudioMixer& mixer)
        {
            m_bLaunchingGame = false;

            m_background.setTexture(m_game.getSharedBackgroundTex());

            m_startBoardTopDrawer.setTexture(m_game.getSharedBackgroundTex(), &m_config.startBoardTopClip);
            m_startBoardTop.setPos(m_config.startBoardTopPos);

            m_digger.setTexture(m_pDiggerTex);
            m_digger.setPos(m_config.diggerPos);
            m_digger.setImpulse(m_config.diggerOutImpulse);
            m_digger.setAcceleration(m_config.diggerOutAcceleration);

            m_startBoard.setTexture(m_pStartBoardTex, &m_config.startBoardClip);
            m_startBoard.setPos(m_config.startBoardPos);
            m_startBoard.setImpulse(m_config.startBoardOutImpulse);
            m_startBoard.setAcceleration(m_config.startBoardOutAcceleration);
            m_startBoard.setOverlay(m_pStartBoardTex, &m_config.startBoardOverlayClip,
                                    m_config.uStartBoardOverlayImgCount, m_config.uStartBoardOverlayImgStride);
            m_startBoard.setOverlayOffset(m_config.startBoardOverlayOffset);
            m_startBoard.setStartButtonRect(m_config.startButtonRect);

            if (m_pStartMusic)
                m_pStartMusic->play();
        }

        void StartScreen::onGameScreenEnd(sys::AudioMixer& mixer)
        {
            mixer.stopMusic();
        }

        void StartScreen::onMouseButtonDown(const sys::Vec2& pos)
        {
            if (!m_bLaunchingGame)
                m_startBoard.onMouseButtonDown(pos);
        }

        void StartScreen::onMouseButtonUp(const sys::Vec2& pos)
        {
            if (!m_bLaunchingGame)
                m_startBoard.onMouseButtonUp(pos);
        }

        void StartScreen::onMouseMove(const sys::Vec2& pos, bool bDragging)
        {
            if (!m_bLaunchingGame)
                m_startBoard.onMouseMove(pos, bDragging);
        }

        void StartScreen::update(const sys::FrameInfo& frame)
        {
            if (m_bLaunchingGame)
            {
                m_digger.update(frame);
                m_startBoard.update(frame);

                if (m_digger.isLeftUpOut() && m_startBoard.isLeftUpOut())
                    m_game.switchToNextScreen();
            }
        }

        void StartScreen::draw(sys::Renderer& rdr)
        {
            rdr.draw(m_background);
            rdr.draw(m_startBoard);
            rdr.draw(m_startBoardTop);
            rdr.draw(m_digger);
        }

        void StartScreen::playButtonSample() const
        {
            sys::AudioSample* pSample = m_game.getSharedButtonSample();
            if (pSample)
                pSample->play();
        }

        void StartScreen::launchGame()
        {
            m_bLaunchingGame = true;

            if (m_pStartSample)
                m_pStartSample->play();
        }
    } // namespace start
} // namespace game

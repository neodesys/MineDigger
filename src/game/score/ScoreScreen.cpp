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

#include "../../sys/FrameInfo.h"
#include "../../sys/Music.h"
#include "../../sys/Renderer.h"
#include "../../sys/Texture.h"

namespace
{
    const char SCREEN_NAME[] = "Score";
}

namespace game
{
    namespace score
    {
        ScoreScreen::ScoreScreen(MineDigger& game)
            : m_game(game), m_config(game.getConfig().scoreScreenConfig), m_backButton(*this)
        {
        }

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
            if (!m_pScoreBackTex)
            {
                if (pEngine)
                {
                    m_pScoreBackTex = sys::Texture::loadTexture(*pEngine, m_config.scoreBackAsset);
                    if (!m_pScoreBackTex)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            if (!m_pScoreTex)
            {
                if (pEngine)
                {
                    m_pScoreTex = sys::Texture::loadTexture(*pEngine, m_config.scoreAsset);
                    if (!m_pScoreTex)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            if (!m_pScoreMusic)
            {
                if (pEngine)
                {
                    m_pScoreMusic = sys::Music::loadMusic(*pEngine, m_config.scoreMusicAsset);
                    if (!m_pScoreMusic)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            if (!m_pExploseSample)
            {
                if (pEngine)
                {
                    m_pExploseSample = sys::AudioSample::loadSample(*pEngine, m_config.exploseSampleAsset);
                    if (!m_pExploseSample)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            if (!m_pVictorySample)
            {
                if (pEngine)
                {
                    m_pVictorySample = sys::AudioSample::loadSample(*pEngine, m_config.victorySampleAsset);
                    if (!m_pVictorySample)
                    {
                        cleanRes(true);
                        return app::ResState::ERROR;
                    }
                }

                return app::ResState::LOADING;
            }

            return app::ResState::READY;
        }

        void ScoreScreen::cleanRes(bool bForce)
        {
            if (bForce)
            {
                delete m_pVictorySample;
                m_pVictorySample = nullptr;

                delete m_pExploseSample;
                m_pExploseSample = nullptr;

                delete m_pScoreMusic;
                m_pScoreMusic = nullptr;

                delete m_pScoreTex;
                m_pScoreTex = nullptr;

                delete m_pScoreBackTex;
                m_pScoreBackTex = nullptr;
            }
        }

        void ScoreScreen::onGameScreenStart(sys::AudioMixer& mixer)
        {
            m_animState = AnimState::EXPLODE;
            m_uAnimStartTimestamp = 0;

            m_explosionColor = m_config.explosionColor;
            m_explosionSpeedinMs = m_config.explosionSpeedinMs;
            if (m_explosionSpeedinMs <= 0.f)
                m_explosionSpeedinMs = 1.f;

            m_background.setTexture(m_pScoreBackTex);

            m_scoreSprite.setTexture(m_pScoreTex, &m_config.scoreClip);
            m_scoreSprite.setNumberStamp(m_game.getSharedNumberStamp());
            m_scoreSprite.setNumberStampOffset(m_config.scoreNumberStampOffset);
            m_scoreSprite.setMinDigits(m_config.scoreMinDigits);
            m_scoreSprite.setColor(m_config.scoreColor);
            m_scoreSprite.setShadowColor(m_config.scoreShadowColor);
            m_scoreSprite.setShadowOffset(m_config.scoreShadowOffset[0], m_config.scoreShadowOffset[1]);
            m_scoreSprite.setPos(m_config.scoreInitialPos);
            m_scoreSprite.setFinalPos(m_config.scoreFinalPos);
            m_scoreSprite.startAnim(m_config.uScoreAnimDuration);

            m_backButton.setPos(m_config.backButtonPos);
            m_backButton.setTexture(m_pScoreTex, &m_config.backButtonClip);
            m_backButton.setOverlay(m_pScoreTex, &m_config.backButtonOverlayClip, m_config.uBackButtonOverlayImgCount,
                                    m_config.uBackButtonOverlayImgStride);
            m_backButton.setBackButtonRect(m_config.backButtonRect);

            if (m_pExploseSample)
                m_pExploseSample->play();
        }

        void ScoreScreen::onGameScreenEnd(sys::AudioMixer& mixer)
        {
            mixer.stopAllSamples();
            mixer.stopMusic();
        }

        void ScoreScreen::onMouseButtonDown(const sys::Vec2& pos)
        {
            if (m_animState == AnimState::BACK_BUTTON)
                m_backButton.onMouseButtonDown(pos);
        }

        void ScoreScreen::onMouseButtonUp(const sys::Vec2& pos)
        {
            if (m_animState == AnimState::BACK_BUTTON)
                m_backButton.onMouseButtonUp(pos);
        }

        void ScoreScreen::onMouseMove(const sys::Vec2& pos, bool bDragging)
        {
            if (m_animState == AnimState::BACK_BUTTON)
                m_backButton.onMouseMove(pos, bDragging);
        }

        void ScoreScreen::update(const sys::FrameInfo& frame)
        {
            switch (m_animState)
            {
            case AnimState::EXPLODE: {
                m_scoreSprite.update(frame);

                sys::AudioSample* pSample = m_game.getSharedSuccessSample();
                if (pSample && (m_sampleTracker.getStatus() == sys::AudioStatus::STOPPED))
                    pSample->play(0, &m_sampleTracker);

                if (!m_uAnimStartTimestamp)
                    m_uAnimStartTimestamp = frame.getTimestamp();
                else
                {
                    unsigned long t = frame.getTimestamp();
                    if (t > m_uAnimStartTimestamp)
                    {
                        float units = (t - m_uAnimStartTimestamp) * m_explosionSpeedinMs;
                        if (units < m_config.explosionColor.a)
                            m_explosionColor.a = m_config.explosionColor.a - static_cast<unsigned char>(units);
                        else
                        {
                            m_explosionColor.a = 0;
                            m_animState = AnimState::WAITING_SCORE;
                        }
                    }
                }
            }
            break;

            case AnimState::WAITING_SCORE:
                m_scoreSprite.update(frame);

                if (m_scoreSprite.hasAnimFinished())
                {
                    if (m_pVictorySample)
                        m_pVictorySample->play(0, &m_sampleTracker);

                    m_animState = AnimState::BACK_BUTTON;
                }
                else
                {
                    sys::AudioSample* pSample = m_game.getSharedSuccessSample();
                    if (pSample && (m_sampleTracker.getStatus() == sys::AudioStatus::STOPPED))
                        pSample->play(0, &m_sampleTracker);
                }
                break;

            case AnimState::BACK_BUTTON:
                if (m_pScoreMusic && (m_pScoreMusic->getStatus() != sys::AudioStatus::PLAYING) &&
                    (m_sampleTracker.getStatus() == sys::AudioStatus::STOPPED))
                    m_pScoreMusic->play();
                break;
            }
        }

        void ScoreScreen::draw(sys::Renderer& rdr)
        {
            rdr.draw(m_background);
            rdr.draw(m_scoreSprite);

            switch (m_animState)
            {
            case AnimState::EXPLODE:
                rdr.drawColoredRect(m_explosionColor, sys::BlendMode::ADDITIVE, nullptr);
                break;

            case AnimState::BACK_BUTTON:
                rdr.draw(m_backButton);
                break;

            default:
                break;
            }
        }

        void ScoreScreen::playButtonSample() const
        {
            sys::AudioSample* pSample = m_game.getSharedButtonSample();
            if (pSample)
                pSample->play();
        }

        void ScoreScreen::playAgain()
        {
            m_game.switchToNextScreen();
        }
    } // namespace score
} // namespace game

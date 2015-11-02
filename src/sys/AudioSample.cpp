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

#include "AudioSample.h"

#include <cassert>
#include <new>

#include <SDL2/SDL_mixer.h>

#include "GameEngine.h"
#include "ResLoader.h"
#include "AudioMixer.h"

namespace sys
{
	const Logger AudioSample::s_log("AudioSample");

	AudioSample* AudioSample::loadSample(const GameEngine& engine, const char* asset)
	{
		Mix_Chunk* pSDLSample = engine.getResLoader().loadSample(asset);
		if (!pSDLSample)
			return nullptr;

		AudioSample* pSample = new(std::nothrow) AudioSample(pSDLSample);
		if (!pSample)
		{
			Mix_FreeChunk(pSDLSample);
			s_log.critical("Out of memory");
			return nullptr;
		}

		s_log.info("Audio sample loaded for \"%s\"", asset);
		return pSample;
	}

	AudioSample::~AudioSample()
	{
		assert(m_pSDLSample);
		Mix_FreeChunk(m_pSDLSample);
	}

	bool AudioSample::play(int repeat, Tracker* pTracker)
	{
		if (repeat >= FOREVER)
		{
			assert(m_pSDLSample);
			int channel = Mix_PlayChannel(-1, m_pSDLSample, repeat);
			if (channel >= 0)
			{
				if (pTracker)
				{
					pTracker->m_trackedChannel = channel;
					pTracker->m_trackedChunk = m_pSDLSample;
				}

				return true;
			}

			s_log.warning("Cannot play audio sample (%s)", Mix_GetError());
		}

		if (pTracker)
		{
			pTracker->m_trackedChannel = -1;
			pTracker->m_trackedChunk = nullptr;
		}

		return false;
	}

	AudioStatus AudioSample::Tracker::getStatus()
	{
		if ((m_trackedChannel >= 0) && m_trackedChunk &&
			Mix_Playing(m_trackedChannel) &&
			(Mix_GetChunk(m_trackedChannel) == m_trackedChunk))
		{
			if (Mix_Paused(m_trackedChannel))
				return AudioStatus::PAUSED;
			else
				return AudioStatus::PLAYING;
		}

		m_trackedChannel = -1;
		m_trackedChunk = nullptr;

		return AudioStatus::STOPPED;
	}

	void AudioSample::Tracker::pause()
	{
		if ((m_trackedChannel >= 0) && m_trackedChunk &&
			Mix_Playing(m_trackedChannel) &&
			(Mix_GetChunk(m_trackedChannel) == m_trackedChunk))
		{
			Mix_Pause(m_trackedChannel);
			return;
		}

		m_trackedChannel = -1;
		m_trackedChunk = nullptr;
	}

	void AudioSample::Tracker::resume()
	{
		if ((m_trackedChannel >= 0) && m_trackedChunk &&
			Mix_Playing(m_trackedChannel) &&
			(Mix_GetChunk(m_trackedChannel) == m_trackedChunk))
		{
			Mix_Resume(m_trackedChannel);
			return;
		}

		m_trackedChannel = -1;
		m_trackedChunk = nullptr;
	}

	void AudioSample::Tracker::stop()
	{
		if ((m_trackedChannel >= 0) && m_trackedChunk &&
			Mix_Playing(m_trackedChannel) &&
			(Mix_GetChunk(m_trackedChannel) == m_trackedChunk))
			Mix_HaltChannel(m_trackedChannel);

		m_trackedChannel = -1;
		m_trackedChunk = nullptr;
	}

	void AudioSample::setVolume(float vol)
	{
		int v = 0;
		if (vol > 0.f)
			v = static_cast<int>(MIX_MAX_VOLUME * vol);

		assert(m_pSDLSample);
		Mix_VolumeChunk(m_pSDLSample, v);
	}

	float AudioSample::getVolume() const
	{
		assert(m_pSDLSample);
		int vol = Mix_VolumeChunk(m_pSDLSample, -1);
		if (vol > 0)
			return static_cast<float>(vol) / MIX_MAX_VOLUME;
		else
			return 0.f;
	}
}

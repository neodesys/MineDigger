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

#include "Music.h"

#include <new>

#include <SDL2/SDL_mixer.h>

namespace sys
{
	const Logger AudioMixer::s_log("AudioMixer");

	AudioMixer* AudioMixer::createAudioMixer(int freq, int polyphony, int bufferSize)
	{
		if (freq <= 0)
			freq = 44100;

		if (polyphony < 0)
			polyphony = 8;

		if (bufferSize <= 0)
			bufferSize = 2048;

		if (Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, 2, bufferSize))
		{
			s_log.critical("Cannot create SDL2 audio mixer at %d Hz with %d bytes buffer (%s)", freq, bufferSize, Mix_GetError());
			return nullptr;
		}

		if (!Mix_QuerySpec(&freq, nullptr, nullptr))
		{
			Mix_CloseAudio();
			s_log.critical("Cannot get SDL2 audio mixer information (%s)", Mix_GetError());
			return nullptr;
		}

		polyphony = Mix_AllocateChannels(polyphony);

		AudioMixer* pMixer = new(std::nothrow) AudioMixer(freq, polyphony);
		if (!pMixer)
		{
			Mix_CloseAudio();
			s_log.critical("Out of memory");
			return nullptr;
		}

		s_log.info("Audio mixer created at %d Hz with %d polyphonic channels and %d bytes buffer", freq, polyphony, bufferSize);
		return pMixer;
	}

	AudioMixer::~AudioMixer()
	{
		//stopMusic() is just called to set the Music::s_pActiveMusic to
		//nullptr, as Mix_CloseAudio() will stop anything playing anyway
		stopMusic();
		Mix_CloseAudio();
	}

	void AudioMixer::pauseAllSamples()
	{
		Mix_Pause(-1);
	}

	void AudioMixer::resumeAllSamples()
	{
		Mix_Resume(-1);
	}

	void AudioMixer::stopAllSamples()
	{
		Mix_HaltChannel(-1);
	}

	void AudioMixer::setSamplesGlobalVolume(float vol)
	{
		int v = 0;
		if (vol > 0.f)
			v = static_cast<int>(MIX_MAX_VOLUME * vol);

		Mix_Volume(-1, v);
	}

	float AudioMixer::getSamplesGlobalVolume() const
	{
		int vol = Mix_Volume(-1, -1);
		if (vol > 0)
			return static_cast<float>(vol) / MIX_MAX_VOLUME;
		else
			return 0.f;
	}

	AudioStatus AudioMixer::getMusicStatus() const
	{
		if (Mix_PlayingMusic())
		{
			if (Mix_PausedMusic())
				return AudioStatus::PAUSED;
			else
				return AudioStatus::PLAYING;
		}
		else
			return AudioStatus::STOPPED;
	}

	void AudioMixer::pauseMusic()
	{
		Mix_PauseMusic();
	}

	void AudioMixer::resumeMusic()
	{
		Mix_ResumeMusic();
	}

	void AudioMixer::stopMusic()
	{
		Mix_HaltMusic();
		Music::s_pActiveMusic = nullptr;
	}

	void AudioMixer::setMusicVolume(float vol)
	{
		int v = 0;
		if (vol > 0.f)
			v = static_cast<int>(MIX_MAX_VOLUME * vol);

		Mix_VolumeMusic(v);
	}

	float AudioMixer::getMusicVolume() const
	{
		int vol = Mix_VolumeMusic(-1);
		if (vol > 0)
			return static_cast<float>(vol) / MIX_MAX_VOLUME;
		else
			return 0.f;
	}
}

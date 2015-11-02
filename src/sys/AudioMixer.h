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

#ifndef _AUDIOMIXER_H_
#define	_AUDIOMIXER_H_

#include "Logger.h"

namespace sys
{
	enum struct AudioStatus : unsigned char
	{
		PLAYING,
		PAUSED,
		STOPPED
	};

	class AudioMixer final
	{
	public:
		//Pass -1 for default values
		//Default frequency is 44100 Hz
		//Default polyphony is 8 channels
		//Default buffer size is 2048 bytes (a big buffer increases latency
		//while a small one uses more CPU and augments the risk of jerky sound
		//reproduction on slow machines)
		static AudioMixer* createAudioMixer(int freq = -1, int polyphony = -1, int bufferSize = -1);
		~AudioMixer();

		const int m_frequency;
		const int m_polyphony;

		//Audio samples
		void pauseAllSamples();
		void resumeAllSamples();
		void stopAllSamples();

		//Volume is between 0.f and 1.f
		//This controls the samples global volume, it will be mixed with each
		//sample independent volume on playing
		void setSamplesGlobalVolume(float vol);
		float getSamplesGlobalVolume() const;

		//Music
		AudioStatus getMusicStatus() const;

		void pauseMusic();
		void resumeMusic();
		void stopMusic();

		void setMusicVolume(float vol);
		float getMusicVolume() const;

	private:
		AudioMixer(int frequency, int polyphony) : m_frequency(frequency), m_polyphony(polyphony) {}
		AudioMixer(const AudioMixer&) = delete;
		AudioMixer& operator=(const AudioMixer&) = delete;

		static const Logger s_log;
	};
}

#endif //_AUDIOMIXER_H_

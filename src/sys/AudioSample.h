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

#ifndef _AUDIOSAMPLE_H_
#define	_AUDIOSAMPLE_H_

#include "Logger.h"

struct Mix_Chunk;

namespace sys
{
	class GameEngine;
	enum struct AudioStatus : unsigned char;

	class AudioSample final
	{
	public:
		static AudioSample* loadSample(const GameEngine& engine, const char* asset);
		~AudioSample();

		enum : int
		{
			FOREVER = -1
		};

		class Tracker;

		//The tracker allows to track and modify the sample playing state.
		//Calling play() starts playing the sample on the first polyphonic
		//channel available, then if no tracker has been provided the sample
		//will continue playing until it finishes or the global
		//AudioMixer::stopAllSamples() is called. Passing a valid tracker
		//allows to control the sample independently.
		//Thus, if sample is short and just played once the tracker is not
		//really useful, but if you repeat the sample (especially FOREVER) it
		//is a good idea to use a tracker to keep control on the playing state.
		//Passing an already used tracker will just override the tracker state
		//and you will loose control on the previously tracked sample.
		//
		//WARNING: the provided tracker state is always overriden, even if
		//calling play() fails.
		bool play(int repeat = 0, Tracker* pTracker = nullptr);

		class Tracker final
		{
		public:
			AudioStatus getStatus();

			void pause();
			void resume();
			void stop();

		private:
			int m_trackedChannel = -1;
			Mix_Chunk* m_trackedChunk = nullptr;

			friend bool AudioSample::play(int, Tracker*);
		};

		//Volume is between 0.f and 1.f
		//This controls the sample independent volume, it will be mixed with
		//the samples global volume on playing
		void setVolume(float vol);
		float getVolume() const;

	private:
		AudioSample(Mix_Chunk* pSDLSample) : m_pSDLSample(pSDLSample) {}
		AudioSample(const AudioSample&) = delete;
		AudioSample& operator=(const AudioSample&) = delete;

		Mix_Chunk* const m_pSDLSample;

		static const Logger s_log;
	};
}

#endif //_AUDIOSAMPLE_H_

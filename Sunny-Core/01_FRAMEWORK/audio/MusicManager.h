#pragma once

#include "../include.h"
#include "Music.h"

namespace sunny
{
	namespace audio
	{
		typedef std::map<int, FMOD::Channel*> channelMap;
		typedef std::map<int,    std::string>    nameMap;

		class MusicManager
		{
		private:
			static const int MAX_CHANNEL_EX = 32;

		private:
			static FMOD::Studio::System* studioSystem;
			static FMOD::System*         system;

			static channelMap  channelMap;
			static    nameMap     nameMap;

		private:
			friend class audio::Music;

			static std::vector<Music*> m_musics;

			MusicManager() = default;

			static Music* Add(Music* music);

		public:
			static void Init();

			static Music* Get(const std::string& name);

			static void Update();

			static void Clean();

		private:
			static int GetNextChannelID();
		};
	}
}
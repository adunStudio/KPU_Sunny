#include "MusicManager.h"

namespace sunny
{
	namespace audio
	{
		FMOD::Studio::System* MusicManager::studioSystem = nullptr;
		FMOD::System* MusicManager::system               = nullptr;

		std::vector<Music*> MusicManager::m_musics;

		channelMap MusicManager::channelMap;
	   	   nameMap MusicManager::   nameMap;

		void MusicManager::Init()
		{
			FMOD::Studio::System::create(&studioSystem);

			studioSystem->initialize(MAX_CHANNEL_EX, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL);

			studioSystem->getLowLevelSystem(&system);
		}

		Music* MusicManager::Add(Music* music)
		{
			m_musics.push_back(music);

			return music;
		}

		Music* MusicManager::Get(const std::string& name)
		{
			for (Music* music : m_musics)
			{
				if (music->GetName() == name)
					return music;
			}

			return nullptr;
		}

		void MusicManager::Update()
		{
			for (int i = 0; i < MAX_CHANNEL_EX; ++i)
			{
				if (channelMap[i] == nullptr) return;
				
				bool isPlaying = false;

				channelMap[i]->isPlaying(&isPlaying);

				if (!isPlaying)
				{
					channelMap[i] = nullptr;
					nameMap[i]    = "";
				}
			}
			
			system->update();
		}

		void MusicManager::Clean()
		{
			for (int i = 0; i < m_musics.size(); ++i)
				delete m_musics[i];

			studioSystem->unloadAll();
			studioSystem->release();
		}

		int MusicManager::GetNextChannelID()
		{
			for (int i = 0; i < MAX_CHANNEL_EX; ++i)
			{
				if (channelMap[i] == nullptr)
					return i;
			}

			return -1;
		}
	}
}

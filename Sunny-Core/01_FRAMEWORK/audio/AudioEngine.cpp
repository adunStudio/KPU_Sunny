#include "AudioEngine.h"

namespace sunny
{
	namespace audio
	{
		Implementation::Implementation()
		{
			m_studioSystem = nullptr;
			
			FMOD::Studio::System::create(&m_studioSystem);

			m_studioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL);

			m_system = nullptr;

			m_studioSystem->getLowLevelSystem(&m_system);
		}

		Implementation::~Implementation()
		{
			m_studioSystem->unloadAll();
			m_studioSystem->release();
		}

		void Implementation::Update()
		{
			vector<map<int, FMOD::Channel*>::iterator> stoppedChannels;

			for (auto it = m_channels.begin(), it_end = m_channels.end(); it != it_end; ++it)
			{
				bool isPlaying = false;

				it->second->isPlaying(&isPlaying);
			
				if (!isPlaying)
					stoppedChannels.push_back(it);
			}

			for (auto& it : stoppedChannels)
			{
				m_channels.erase(it);
			}

			m_system->update();
		}

		Implementation* AudioEngine::s_implementation = nullptr;

		void AudioEngine::Init()
		{
			s_implementation = new Implementation();
		};

		void AudioEngine::Update()
		{
			s_implementation->Update();
		};

		void AudioEngine::Clear()
		{
			delete s_implementation;
		};

		void AudioEngine::LoadSound(const string& strSoundName, bool b3d, bool bLooping, bool bStream)
		{
			auto it = s_implementation->m_sounds.find(strSoundName);
			if (it != s_implementation->m_sounds.end())
				return;

			FMOD_MODE eMode = FMOD_DEFAULT;
			eMode |= b3d ? FMOD_3D : FMOD_2D;
			eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
			eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

			FMOD::Sound* sound = nullptr;

			s_implementation->m_system->createSound(strSoundName.c_str(), eMode, nullptr, &sound);

			if (sound)
				s_implementation->m_sounds[strSoundName] = sound;
		}

		void AudioEngine::UnLoadSound(const string& strSoundName)
		{
			auto it = s_implementation->m_sounds.find(strSoundName);
			if (it == s_implementation->m_sounds.end())
				return;

			it->second->release();

			s_implementation->m_sounds.erase(it);
		}

		int AudioEngine::Play(const string& strSoundName, const vec3& pos, float fVolumedB)
		{
			int channelID = s_implementation->nextID++;

			auto it = s_implementation->m_sounds.find(strSoundName);
			
			if (it == s_implementation->m_sounds.end())
			{
				LoadSound(strSoundName);
				
				it = s_implementation->m_sounds.find(strSoundName);

				if (it == s_implementation->m_sounds.end())
				{
					return channelID;
				}
			}

			FMOD::Channel* channel = nullptr;

			s_implementation->m_system->playSound(it->second, nullptr, true, &channel);

			if (channel)
			{
				FMOD_MODE currMode;
				
				it->second->getMode(&currMode);
				
				if (currMode & FMOD_3D) {
					FMOD_VECTOR position = VectorToFmod(pos);

					channel->set3DAttributes(&position, nullptr);
				}

				channel->setVolume(dbToVolume(fVolumedB));

				channel->setPaused(false);

				s_implementation->m_channels[channelID] = channel;
			}

			return channelID;
		}

		float AudioEngine::dbToVolume(float db)
		{
			return powf(10.0f, 0.05f * db);
		}

		FMOD_VECTOR AudioEngine::VectorToFmod(const vec3& pos) {
			FMOD_VECTOR fVec;
			fVec.x = pos.x;
			fVec.y = pos.y;
			fVec.z = pos.z;
			return fVec;
		}
	}

}
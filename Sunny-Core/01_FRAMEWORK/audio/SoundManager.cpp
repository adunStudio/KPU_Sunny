#include "SoundManager.h"

#define GAU_THREAD_POLICY_MULTI 2
#include <ga.h>
#include <gau.h>

namespace sunny
{
	namespace audio
	{
		std::vector<Sound*> SoundManager::m_sounds;

		void SoundManager::Init()
		{
			gc_initialize(0);
			m_manager = gau_manager_create();
			m_mixer   = gau_manager_mixer(m_manager);
		}

		Sound* SoundManager::Add(Sound* sound)
		{
			m_sounds.push_back(sound);

			return sound;
		}

		Sound* SoundManager::Get(const std::string& name)
		{
			for (Sound* sound : m_sounds)
			{
				if (sound->GetName() == name)
					return sound;
			}

			return nullptr;
		}

		void SoundManager::Update()
		{
			gau_manager_update(m_manager);
		}

		void SoundManager::Clean()
		{
			for (int i = 0; i < m_sounds.size(); ++i)
				delete m_sounds[i];

			gau_manager_destroy(m_manager);
			gc_shutdown();
		}
	}
}
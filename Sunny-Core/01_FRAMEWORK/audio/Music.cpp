#include "Music.h"

#include "MusicManager.h"

namespace sunny
{
	namespace audio
	{
		Music::Music(const std::string& name, const std::string& filename)
		: m_name(name), m_filename(filename), m_mode(FMOD_DEFAULT)
		{
			std::vector<std::string> split = utils::SplitString(m_filename, '.');
			if (split.size() < 2)
			{
				std::cout << "[Music] Invalid file name '" << m_filename << "'!" << std::endl;
				return;
			}

			if (MusicManager::Get(name))
			{
				std::cout << "[Music] Overlaped name: '" << m_name << "!" << std::endl;
				return;
			}

			std::string physicalPath;

			// 파일 존재유무 확인
			if (!VFS::Get()->ResolvePhysicalPath(m_filename, physicalPath))
			{
				std::cout << "[Music] Could not load file '" << m_filename << "'!" << std::endl;
				exit(1);
			}

			MusicManager::system->createSound(physicalPath.c_str(), m_mode, nullptr, &m_sound);
			
			if (m_sound == nullptr)
			{
				std::cout << "[Music] Is Not Sound file name: '" << m_filename << "!" << std::endl;
				return;
			}

			MusicManager::Add(this);
		}

		Music::~Music()
		{
			m_sound->release();
		}

		void Music::Play(bool loop)
		{
			if (m_sound == nullptr) return;
			
			int id =  MusicManager::GetNextChannelID();
			
			if (id == -1) return;

			FMOD::Channel* channel = nullptr;

			m_mode = FMOD_DEFAULT;
			m_mode |= FMOD_2D;
			m_mode |= FMOD_CREATECOMPRESSEDSAMPLE;
			m_mode |= loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

			m_sound->setMode(m_mode);

			MusicManager::system->playSound(m_sound, nullptr, true, &channel);

			channel->setPaused(false);

			MusicManager::channelMap[id] = channel;
			MusicManager::   nameMap[id] = m_name;
		}

		void Music::Loop()
		{
			Play(true);
		}

		void Music::Pause()
		{
			for (int i = 0; i < MusicManager::MAX_CHANNEL_EX; ++i)
			{
				if (MusicManager::nameMap[i] == m_name)
				{
					MusicManager::channelMap[i]->setPaused(true);
				}
			}
		}

		void Music::Stop()
		{
			for (int i = 0; i < MusicManager::MAX_CHANNEL_EX; ++i)
			{
				if (MusicManager::nameMap[i] == m_name)
				{
					MusicManager::channelMap[i]->stop();
				}
			}
		}

		void Music::Resume()
		{
			for (int i = 0; i < MusicManager::MAX_CHANNEL_EX; ++i)
			{
				if (MusicManager::nameMap[i] == m_name)
				{
					MusicManager::channelMap[i]->setPaused(false);
				}
			}
		}
	}
}
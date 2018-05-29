#include "Sound.h"

#include "SoundManager.h"




namespace sunny
{
	namespace audio
	{
		Sound::Sound(const std::string& name, const std::string& filename)
		: m_name(name), m_filename(filename), m_playing(false), m_count(0)
		{
			std::vector<std::string> split = utils::SplitString(m_filename, ".");
			
			if (split.size() < 2)
			{
				// Debug System:
				std::cout << "[Sound] Invalid file name '" << m_filename << "'!" << std::endl;
				return;
			}

			m_sound = gau_load_sound_file(filename.c_str(), split.back().c_str());
		
			if (m_sound == nullptr)
			{
				// Debug System:
				std::cout << "[Sound] Could not load file '" << m_filename << "'!" << std::endl;
				return;
			}
		}

		Sound::~Sound()
		{
			ga_sound_release(m_sound);
		}

		void Sound::Play()
		{
			gc_int32 quit = 0;
			m_handle = gau_create_handle_sound(SoundManager::m_mixer, m_sound, &destroy_on_finish, &quit, NULL);
			m_handle->sound = this;
			ga_handle_play(m_handle);
			
			m_count++;
			m_playing = true;
		}

		void Sound::Loop()
		{
			gc_int32 quit = 0;
			m_handle = gau_create_handle_sound(SoundManager::m_mixer, m_sound, &loop_on_finish, &quit, NULL);
			m_handle->sound = this;
			ga_handle_play(m_handle);

			m_playing = true;
		}

		void Sound::Pause()
		{
			if (!m_playing) return;

			m_playing = false;

			ga_handle_stop(m_handle);
		}

		void Sound::Resume()
		{
			if (m_playing) return;
;
			m_playing = true;

			ga_handle_play(m_handle);
		}

		void Sound::Stop()
		{
			Pause();
		}

		void Sound::SetVolume(float volume)
		{
			if (!m_playing)
			{
				std::cout << "[Sound] Cannot set volume! Sound is not currently playing!" << std::endl;
				return;
			}

			m_volume = volume;

			ga_handle_setParamf(m_handle, GA_HANDLE_PARAM_GAIN, volume);
		}



		// friend

		void destroy_on_finish(ga_Handle* in_handle, void* in_context)
		{
			Sound* sound = (Sound*)in_handle->sound;
			sound->m_count--;

			if (sound->m_count == 0)
				sound->Stop();
		}

		void loop_on_finish(ga_Handle* in_handle, void* in_context)
		{
			Sound* sound = (Sound*)in_handle->sound;
			
			sound->Loop();
			
			ga_handle_destroy(in_handle);
		}
	}
}
#pragma once

#include "../include.h"

#include "../utils/String.h"

struct ga_Sound;
struct ga_Handle;

namespace sunny
{
	namespace audio
	{
		class Sound
		{
		private:
			std::string m_name;
			std::string m_filename;

			unsigned int m_count;

			ga_Sound*  m_sound;
			ga_Handle* m_handle;

			int m_position;

			bool m_playing;

			float m_volume;

		public:
			Sound(const std::string& name, const std::string& filename);
			~Sound();

			void Play();
			void Loop();
			void Pause();
			void Resume();
			void Stop();

			void SetVolume(float volume);

			inline const bool         IsPlaying()   const { return m_playing;  }
			inline const float        GetVolume()   const { return m_volume;   }
			inline const std::string& GetName()     const { return m_name;     }
			inline const std::string& GetFileName() const { return m_filename; }

			friend void destroy_on_finish(ga_Handle* in_handle, void* in_context);
			friend void loop_on_finish(ga_Handle* in_handle, void* in_context);
		};
	}
}
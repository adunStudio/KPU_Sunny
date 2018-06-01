#pragma once

#include "../include.h"
#include "../utils/String.h"
#include "../system/VFS.h"
#include "fmod.hpp"
#include "fmod_studio.hpp"

namespace sunny
{
	namespace audio
	{
		class Music
		{
		private:
			FMOD::Sound* m_sound = nullptr;
			FMOD_MODE m_mode;

			std::string m_name;
			std::string m_filename;

		public:
			Music(const std::string& name, const std::string& filename);
			~Music();

			void Play(bool loop = false);
			void Loop();
			void Pause();
			void Stop();
			void Resume();

		public:
			inline const std::string& GetName()     const { return m_name; }
			inline const std::string& GetFileName() const { return m_filename; }
		};
	}
}
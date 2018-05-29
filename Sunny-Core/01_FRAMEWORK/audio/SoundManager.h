#pragma once

#include "../include.h"

#include "Sound.h"

namespace sunny
{
	namespace audio
	{
		class SoundManager
		{
		private:
			friend class Sound;

			static std::vector<Sound*> m_sounds;

		public:
			static void Init();

			static Sound* Add(Sound* sound);

			static Sound* Get(const std::string& name);

			static void Update();

			static void Clean();
		};
	}
}
#pragma once

#include "../include.h"
#include "../maths/vec3.h"

#include "fmod.hpp"
#include "fmod_studio.hpp"


namespace sunny
{
	namespace audio
	{
		using namespace std;

		using namespace FMOD;

		using namespace maths;

		struct Implementation {
			Implementation();
			~Implementation();

			void Update();

			FMOD::Studio::System* m_studioSystem;
			FMOD::        System* m_system;

			int nextID = 0;

			map<string, FMOD::Sound*>                 m_sounds;
			map<int, FMOD::Channel*>                  m_channels;
			map<string, FMOD::Studio::EventInstance*> m_events;
			map<string, FMOD::Studio::Bank*>          m_banks;
		};

		class AudioEngine
		{
		private:
			static Implementation* s_implementation;

		public:
			static void Init();
			static void Update();
			static void Clear();

			static void LoadSound(const string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
			static void UnLoadSound(const string& strSoundName);
			static int  Play(const string& strSoundName, const vec3& pos = vec3{ 0, 0, 0 }, float fVolumedB = 0.0f);

			/*
			void LoadBank(const string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
			void LoadEvent(const string& strEventName);
			
			void Set3dListenerAndOrientation(const vec3& pos = vec3{ 0, 0, 0 }, float fVolumedB = 0.0f);
			void PlayEvent(const string& strEventName);
			void StopChannel(int nChannelId);
			void StopEvent(const string& strEventName, bool bImmediate = false);
			void GeteventParameter(const string& strEventName, const string& strEventParameter, float* parameter);
			void SetEventParameter(const string& strEventName, const string& strParameterName, float fValue);
			void StopAllChannels();
			void SetChannel3dPosition(int nChannelId, const vec3& position);
			void SetChannelvolume(int nChannelId, float fVolumedB);
			bool IsPlaying(int nChannelId) const;
			bool IsEventPlaying(const string& strEventName) const;
			float VolumeTodb(float volume);
			*/
			static float dbToVolume(float db);
			static FMOD_VECTOR VectorToFmod(const vec3& pos);
			
		private:
			AudioEngine() {}

		};
	}
}
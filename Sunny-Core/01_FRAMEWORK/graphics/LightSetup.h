#pragma once

#include "../include.h"
#include "Light.h"

namespace sunny
{
	namespace graphics
	{
		class LightSetup
		{
		private:
			std::vector<Light*> m_lights;

		public:
			LightSetup();
			~LightSetup();

			Light* Add(Light* light);
			void Remove(Light* light);

			inline const std::vector<Light*> GetLights() const { return m_lights; }
		};
	}
}
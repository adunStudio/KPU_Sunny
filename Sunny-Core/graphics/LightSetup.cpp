#include "LightSetup.h"

namespace sunny
{
	namespace graphics
	{
		LightSetup::LightSetup()
		{}

		LightSetup::~LightSetup()
		{
			for (unsigned int i = 0; i < m_lights.size(); ++i)
				delete m_lights[i];
		}

		Light* LightSetup::Add(Light* light)
		{
			m_lights.push_back(light);

			return light;
		}

		void LightSetup::Remove(Light* light)
		{
			for (unsigned int i = 0; i < m_lights.size(); ++i)
			{
				if (m_lights[i] == light)
				{
					m_lights.erase(m_lights.begin() + i);
					break;
				}
			}
		}
	}
}
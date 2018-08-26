#pragma once

#include <SUNNY.h>
#include "Bullet.h"
#include "../07_SERVER/BossLocker.h"

namespace game
{
	struct SCBullet
	{
		vec3& m_position;
		int m_type;

		SCBullet(vec3& position, int type) : m_position(position), m_type(type)
		{

		}
	};

	class SCBulletParticleSystem : public ParticleSystem
	{
	public:
		SCBulletParticleSystem(directx::Texture* texture) : ParticleSystem(texture)
		{
			m_maxParticleCount = 50000;
		}

		void Update(const utils::Timestep& ts) override
		{
			//m_currentParticleCount = BossLocker::bulletList->length;

			int index = 0;

			UpdateBuffers();
		}
	};
}
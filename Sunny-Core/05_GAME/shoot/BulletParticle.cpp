#include "BulletParticle.h"

namespace game
{
	BulletParticle::BulletParticle(directx::Texture* texture)
	: ParticleSystem(texture)
	{
		m_maxParticleCount = 5000;
	}

	void BulletParticle::Update(const utils::Timestep& ts)
	{
		m_currentParticleCount = BossLocker::bulletList->length;

		int index = 0;

		for (PoolIter i(BossLocker::bulletList); i.HasNext(); )
		{
			Bullet* bullet = static_cast<Bullet*>(i.Next());
			
			m_particleList[index].position.x = bullet->GetPosition().x;;
			m_particleList[index].position.y = 30;
			m_particleList[index].position.z = bullet->GetPosition().z;;
			m_particleList[index].color = maths::vec4(1, 0, 0, 1);

			++index;
		}

		UpdateBuffers();
	}
}
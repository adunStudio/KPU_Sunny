#pragma once

#include <SUNNY.h>
#include "Bullet.h"
#include "../07_SERVER/BossLocker.h"

namespace game
{
	class BulletParticle : public ParticleSystem
	{
	public:
		BulletParticle(directx::Texture* texture);

		void Update(const utils::Timestep& ts) override;
	};
}
#pragma once

#include <SUNNY.h>

namespace game
{
	class BulletM : public Entity
	{
	public:
		float m_angle = 0;

		BulletM(Mesh* mesh, float angle)
		: Entity(mesh, RGBA(1, 0, 0, 1)), m_angle(angle)
		{

	
		}

		void Update()
		{

			float x = maths::cos(m_angle) * 35.0f;
			float  z = maths::sin(m_angle) * 35.0f * -1.0f;

			GetTransformComponent()->Translate(vec3(x, 0, z));
		}
	};
};
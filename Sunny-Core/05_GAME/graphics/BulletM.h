#pragma once

#include <SUNNY.h>

namespace game
{
	class BulletM : public Entity
	{
	public:
		float m_angle = 0;
		float x, z;
		bool m_live;
		Animation3D* m_enemy;

		component::TransformComponent* m_t;

		BulletM(Mesh* mesh, float angle, Animation3D* enemy)
		: Entity(mesh, RGBA(1, 0, 0, 1)), m_angle(angle), m_live(true), m_enemy(enemy)
		{
			m_t = GetTransformComponent();
	
		}

		void Update()
		{
			if (!m_live) return;

			float m_x = m_t->GetPosition().x;
			float m_z = m_t->GetPosition().z;

			float x = maths::cos(m_angle) * 35.0f;
			float z = maths::sin(m_angle) * 35.0f * -1.0f;

			m_t->Translate(vec3(x, 0, z));
			
			if (maths::sqrt(pow(m_x - (-300.0f), 2) + pow(m_z - 0, 2)) < 100)
			{
				m_live = false;
				SetVisible(false);
				m_enemy->hp -= 1;
			}

			if ((m_x > -310 && m_x < -290 && m_z > -5 && m_z < 5) || (m_x > 3200 || m_x < -3200 || m_z > 1600 || m_z < -1600))
			{
				m_live = false;
				SetVisible(false);
			}
		}
	};
};
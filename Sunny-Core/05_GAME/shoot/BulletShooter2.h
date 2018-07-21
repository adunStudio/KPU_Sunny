#pragma once
#include <SUNNY.h>
#include "Bullet.h"

namespace game
{
	// NWay ź
	class BulletNWayShooter : public BulletShooter
	{
	public:
		static BulletNWayShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRate;
		float m_shootSpeed;

	private:
		BulletNWayShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed);

	public:
		void Update() override;
	};
}
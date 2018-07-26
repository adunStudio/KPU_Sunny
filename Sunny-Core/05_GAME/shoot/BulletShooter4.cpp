#include "BulletShooter4.h"

namespace game
{
	// ½ÉÇÃÈ£¹ÖÅº
	BulletSimpleHomingShooter* BulletSimpleHomingShooter::Get(const maths::vec3& position)
	{
		return new BulletSimpleHomingShooter(position, 2000, 20);
	}
	BulletSimpleHomingShooter::BulletSimpleHomingShooter(const maths::vec3& position, float shootSpeed, int interval)
		: BulletShooter(position), m_shootSpeed(shootSpeed), m_interval(interval), m_time(0)
	{

	}
	void BulletSimpleHomingShooter::Update()
	{
		if (m_time == 0 && m_transformComponent)
		{
			new SimpleHomingBullet(BULLET_TYPE::AIMING, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position), m_shootSpeed, m_transformComponent);
		}

		m_time = (m_time + 1) % m_interval;
	}

	// ÄÁ½ºÆ®·»ÀÎµåÈ¨À×Åº
	BulletConstrainedHomingShooter* BulletConstrainedHomingShooter::Get(const maths::vec3& position)
	{
		return new BulletConstrainedHomingShooter(position, 600, 20, 0.005f);
	}
	BulletConstrainedHomingShooter::BulletConstrainedHomingShooter(const maths::vec3& position, float shootSpeed, int interval, float maxAngleRate)
		: BulletSimpleHomingShooter(position, shootSpeed, interval), m_maxAngleRate(maxAngleRate)
	{

	}
	void BulletConstrainedHomingShooter::Update()
	{
		if (m_time == 0 && m_transformComponent)
		{
			new ConstrainedHomingBullet(BULLET_TYPE::AIMING, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position), m_shootSpeed, m_maxAngleRate, m_transformComponent);
		}

		m_time = (m_time + 1) % m_interval;
	}

	// °¸Åº
	BulletGapShooter* BulletGapShooter::Get(const maths::vec3& position)
	{
		return new BulletGapShooter(position, 0, 0.95f, 300, 100, 150, 0, 0);
	}
	BulletGapShooter::BulletGapShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootSpeed, int shootCount, int interval, float bulletAngleRate, float bulletSpeedRate)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_bulletAngleRate(bulletAngleRate), m_bulletSpeedRate(bulletSpeedRate), m_time(0)

	{

	}
	void BulletGapShooter::Update()
	{
		if (m_time == 0)
		{
			m_shootAngle = ((float)rand() / (RAND_MAX + 1.0f));

			if (m_shootCount > 1)
			{
				for (int i = 0; i < m_shootCount; ++i)
					new Bullet(BULLET_TYPE::NWAY, m_position, m_shootAngle + m_shootAngleRange * ((float)i / (m_shootCount - 1) - 0.5f), m_bulletAngleRate, m_shootSpeed, m_bulletSpeedRate);
			}
			else
			{
				if (m_shootCount == 1)
					new Bullet(BULLET_TYPE::NWAY, m_position, m_shootAngle, m_bulletAngleRate, m_shootSpeed, m_bulletSpeedRate);
			}
		}

		m_time = (m_time + 1) % m_interval;
	}
}


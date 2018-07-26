#include "BulletShooter3.h"

namespace game
{
	// À¯µµÅº
	BulletAimingShooter* BulletAimingShooter::Get(const maths::vec3& position)
	{
		return new BulletAimingShooter(position, 2000, 10);
	}
	BulletAimingShooter::BulletAimingShooter(const maths::vec3& position, float shootSpeed, int interval)
		: BulletShooter(position), m_shootSpeed(shootSpeed), m_interval(interval), m_time(0)
	{

	}
	void BulletAimingShooter::Update()
	{
		if (m_time == 0 && m_transformComponent)
		{
			new Bullet(BULLET_TYPE::AIMING, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position), 0, m_shootSpeed, 0);
		}

		m_time = (m_time + 1) % m_interval;
	}

	// ¶óÀÎÅº
	BulletLineShooter* BulletLineShooter::Get(const maths::vec3& position)
	{
		return new BulletLineShooter(position, 0, 600, 2, 20, 10);
	}
	BulletLineShooter::BulletLineShooter(const maths::vec3& position, float shootAngle, float shootSpeed, int interval, int shootTime, int waitTime)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootSpeed(shootSpeed), m_interval(interval), m_time(0), m_shootTime(shootTime), m_waitTime(waitTime)
	{

	}
	void BulletLineShooter::Update()
	{
		if (m_time == 0 && m_transformComponent)
		{
			m_shootAngle = GetMyShipAngle(m_transformComponent->GetPosition(), m_position);
		}

		if (m_time < m_shootTime && m_time % m_interval == 0)
		{
			new Bullet(BULLET_TYPE::LINE, m_position, m_shootAngle, 0, m_shootSpeed, 0);
		}

		m_time = (m_time + 1) % (m_shootTime + m_waitTime);
	}

	// ½ºÇÁ·¹µùÅº
	BulletSpreadingShooter* BulletSpreadingShooter::Get(const maths::vec3& position)
	{
		return new BulletSpreadingShooter(position, 0.2f, 1000, 9, 240, 250, 4);
	}
	BulletSpreadingShooter::BulletSpreadingShooter(const vec3& position, float shootAngleRange, float shootSpeed, int shootCount, int interval, float groupSpeed, int groupCount)
		: BulletShooter(position), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_time(0), m_groupSpeed(groupSpeed), m_groupCount(groupCount)
	{

	}
	void BulletSpreadingShooter::Update()
	{
		if (m_time == 0 && m_transformComponent)
		{
			for (int j = 0; j < m_groupCount; ++j)
			{
				if (m_shootCount > 1)
				{
					for (int i = 0; i < m_shootCount; ++i)
						new Bullet(BULLET_TYPE::NWAY, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position) + m_shootAngleRange * ((float)i / (m_shootCount - 1) - 0.5f), 0, m_shootSpeed + m_groupSpeed * j, 0);
				}
				else
				{
					if (m_shootCount == 1)
						new Bullet(BULLET_TYPE::NWAY, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position), 0, m_shootSpeed + m_groupSpeed * j, 0);
				}
			}
		}

		m_time = (m_time + 1) % m_interval;
	}

	// ·£´ý ½ºÇÁ·¹µùÅº
	BulletRandomSpreadingShooter* BulletRandomSpreadingShooter::Get(const maths::vec3& position)
	{
		return new BulletRandomSpreadingShooter(position, 0.2f, 1200, 200, 30, 240);
	}
	BulletRandomSpreadingShooter::BulletRandomSpreadingShooter(const maths::vec3& position, float shootAngleRange, float shootSpeed, float shootSpeedRange, int shootCount, int interval)
		: BulletShooter(position), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_shootSpeedRange(shootSpeedRange), m_shootCount(shootCount), m_interval(interval), m_time(0)
	{

	}
	void BulletRandomSpreadingShooter::Update()
	{
		if (m_time == 0 && m_transformComponent)
		{
			for (int i = 0; i < m_shootCount; ++i)
			{
				new Bullet(BULLET_TYPE::NWAY, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position) + m_shootAngleRange * ((float)rand() / (RAND_MAX + 1.0f) - 0.5f), 0, m_shootSpeed + m_shootSpeedRange * ((float)rand() / (RAND_MAX + 1.0f) - 0.5f), 0);
			}
		}

		m_time = (m_time + 1) % m_interval;
	}

	// ¿À¹öÅ×ÀÌÅ·Åº
	BulletOvertakingShooter* BulletOvertakingShooter::Get(const maths::vec3& position)
	{
		return new BulletOvertakingShooter(position, 0.2f, 1000, 7, 240, 200, 0.003f, 10, 4);
	}
	BulletOvertakingShooter::BulletOvertakingShooter(const maths::vec3& position, float shootAngleRange, float shootSpeed, float shootCount, int interval, int groupSpeed, int groupAngle, int groupCount, int groupInterval)
		: BulletShooter(position), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_groupSpeed(groupSpeed), m_groupAngle(groupAngle), m_groupCount(groupCount), m_groupInterval(groupInterval), m_time(0)
	{

	}
	void BulletOvertakingShooter::Update()
	{
		if (!m_transformComponent) return;

		if (m_time == 0)
		{
			m_shootAngle = GetMyShipAngle(m_transformComponent->GetPosition(), m_position);
		}

		int j = m_time / m_groupInterval;

		if (j < m_groupCount && (m_time % m_groupInterval) == 0)
		{

			if (m_shootCount > 1)
			{
				for (int i = 0; i < m_shootCount; ++i)
					new Bullet(BULLET_TYPE::NWAY, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position) + m_groupAngle * j + m_shootAngleRange * ((float)i / (m_shootCount - 1) - 0.5f), 0, m_shootSpeed + m_groupSpeed * j, 0);
			}
			else
			{
				if (m_shootCount == 1)
					new Bullet(BULLET_TYPE::NWAY, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position), 0, m_shootSpeed + m_groupSpeed * j, 0);
			}
		}

		m_time = (m_time + 1) % m_interval;
	}
}

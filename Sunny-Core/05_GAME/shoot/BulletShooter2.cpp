#include "BulletShooter2.h"


// NWay탄
BulletNWayShooter* BulletNWayShooter::Get(const maths::vec3& position)
{
	return new BulletNWayShooter(position, 0.25f, 0.2f, 2000.0f, 7, 25, 0, 0);
}
BulletNWayShooter::BulletNWayShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootSpeed, int shootCount, int interval, float bulletAngleRate, float bulletSpeedRate)
	: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_bulletAngleRate(bulletAngleRate), m_bulletSpeedRate(bulletSpeedRate), m_time(0)
{

}
void BulletNWayShooter::Update()
{
	if (m_time == 0)
	{
		if (m_shootCount > 1)
		{
			for (int i = 0; i < m_shootCount; ++i)
				new Bullet(BULLET_TYPE::NWAY, m_position, m_shootAngle + m_shootAngleRange * ((float)i / (m_shootCount - 1) - 0.5f), m_bulletAngleRate, m_shootSpeed, m_bulletSpeedRate);
		}
		else
		{
			if(m_shootCount == 1)
				new Bullet(BULLET_TYPE::NWAY, m_position, m_shootAngle, m_bulletAngleRate, m_shootSpeed, m_bulletSpeedRate);
		}
	}

	m_time = (m_time + 1) % m_interval;
}

// Circle NWay탄
BulletCircleNWayShooter::BulletCircleNWayShooter(const maths::vec3& position, float shootAngle, float shootSpeed, float shootCount, int interval, float bulletAngleRate, float bulletSpeedRate)
	: BulletNWayShooter(position, shootAngle, 1.0f - 1.0f/shootCount, shootSpeed, shootCount, interval, bulletAngleRate, bulletSpeedRate)
{

}
BulletCircleNWayShooter* BulletCircleNWayShooter::Get(const maths::vec3& position)
{
	return new BulletCircleNWayShooter(position, 0.25f, 2000, 8, 5, 0, 0);
}

// Bent Circle NWay탄
BulletBentCircleNWayShooter::BulletBentCircleNWayShooter(const maths::vec3& position, float shootAngle, float shootSpeed, float shootCount, int interval, float bulletAngleRate, float bulletSpeedRate)
	: BulletNWayShooter(position, shootAngle, 1.0f - 1.0f / shootCount, shootSpeed, shootCount, interval, bulletAngleRate, bulletSpeedRate)
{

}
BulletBentCircleNWayShooter* BulletBentCircleNWayShooter::Get(const maths::vec3& position)
{
	return new BulletBentCircleNWayShooter(position, 0.25f, 2000, 32, 5, 0.002f, 0);
}

// Aiming Nway탄
BulletAimingNWayShooter* BulletAimingNWayShooter::Get(const maths::vec3& position)
{
	return new BulletAimingNWayShooter(position, 0.25f, 2000.0f, 9, 10);
}
BulletAimingNWayShooter::BulletAimingNWayShooter(const maths::vec3& position, float shootAngleRange, float shootSpeed, int shootCount, int interval)
	: BulletShooter(position), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_time(0)
{
}
void BulletAimingNWayShooter::Update()
{
	if (m_time == 0 && m_transformComponent)
	{
		if (m_shootCount > 1)
		{
			for (int i = 0; i < m_shootCount; ++i)
				new Bullet(BULLET_TYPE::NWAY, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position) + m_shootAngleRange * ((float)i / (m_shootCount - 1) - 0.5f), 0, m_shootSpeed, 0);
		}
		else
		{
			if (m_shootCount == 1)
				new Bullet(BULLET_TYPE::NWAY, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position), 0, m_shootSpeed, 0);
		}
	}

	m_time = (m_time + 1) % m_interval;
}

// Intermittent Aiming NWay탄
BulletIntermittentAimingNWayShooter* BulletIntermittentAimingNWayShooter::Get(const maths::vec3& position)
{
	return new BulletIntermittentAimingNWayShooter(position, 0.25f, 2000, 9, 2, 60, 20);
}
BulletIntermittentAimingNWayShooter::BulletIntermittentAimingNWayShooter(const maths::vec3& position, float shootAngleRange, float shootSpeed, int shootCount, int interval, int shootTime, int waitTime)
	: BulletShooter(position), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_time(0), m_shootTime(shootTime), m_waitTime(waitTime)
{

}
void BulletIntermittentAimingNWayShooter::Update()
{
	if (m_time < m_shootTime && m_time % m_interval == 0 && m_transformComponent)
	{
		for (int i = 0; i < m_shootCount; ++i)
			new Bullet(BULLET_TYPE::NWAY, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position) + m_shootAngleRange * ((float)i / (m_shootCount - 1) - 0.5f), 0, m_shootSpeed, 0);
	}

	m_time = (m_time + 1) % (m_shootTime + m_waitTime);
}

// Random NWay탄
BulletRandomNWayShooter* BulletRandomNWayShooter::Get(const maths::vec3& position)
{
	return new BulletRandomNWayShooter(position, 0.25f, 0.2f, 2000, 1, 1);
}
BulletRandomNWayShooter::BulletRandomNWayShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootSpeed, int shootCount, int interval)
	: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_time(0)
{

}
void BulletRandomNWayShooter::Update()
{
	if (m_time == 0)
	{
		for (int i = 0; i < m_shootCount; ++i)
		{
			new Bullet(BULLET_TYPE::NWAY, m_position, m_shootAngle + m_shootAngleRange * ((float)rand() / (RAND_MAX + 1.0f) - 0.5f), 0, m_shootSpeed, 0);
		}
	}

	m_time = (m_time + 1) % m_interval;
}

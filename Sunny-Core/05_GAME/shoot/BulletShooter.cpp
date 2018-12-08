#include "BulletShooter.h"

namespace game
{
	// Á÷¼±Åº
	BulletDirectionalShooter::BulletDirectionalShooter(const maths::vec3& position, float shootAngle, float shootSpeed)
	: BulletShooter(position), m_shootAngle(shootAngle), m_shootSpeed(shootSpeed)
	{

	}
	BulletDirectionalShooter* BulletDirectionalShooter::Get(const maths::vec3& position)
	{
		return new BulletDirectionalShooter(position, 0.25f, 1500);
	}
	void BulletDirectionalShooter::Update()
	{
		new Bullet(BULLET_TYPE::DIRECTIONAL, m_position, m_shootAngle, 0, m_shootSpeed, 0);
	}

	// ½ºÆÄÀÌ·²Åº
	BulletSpiralShooter::BulletSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRate(shootAngleRate), m_shootSpeed(shootSpeed)
	{

	}
	BulletSpiralShooter* BulletSpiralShooter::Get(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.02f, 1000);
	}
	void BulletSpiralShooter::Update()
	{
		new Bullet(BULLET_TYPE::SPIRAL, m_position, m_shootAngle, 0, m_shootSpeed, 0);
		m_shootAngle += m_shootAngleRate;
		m_shootAngle -= floor(m_shootAngle);
	}

	// ¸ÖÆ¼ ½ºÆÄÀÌ·²Åº
	BulletMultipleSpiralShooter::BulletMultipleSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed, int shootCount)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRate(shootAngleRate), m_shootSpeed(shootSpeed), m_shootCount(shootCount)
	{

	}
	BulletMultipleSpiralShooter* BulletMultipleSpiralShooter::Get(const maths::vec3& position)
	{
		return new BulletMultipleSpiralShooter(position, 0, 0.02f, 5, 4);
	}
	void BulletMultipleSpiralShooter::Update()
	{
		for(int i = 0; i < m_shootCount; ++i)
			new Bullet(BULLET_TYPE::SPIRAL, m_position, m_shootAngle + static_cast<float>(i) / static_cast<float>(m_shootCount), 0, m_shootSpeed, 0);
		m_shootAngle += m_shootAngleRate;
		m_shootAngle -= floor(m_shootAngle);
	}

	// ÀÎÅÍ¹ú ¸ÖÆ¼ ½ºÆÄÀÌ·²Åº
	BulletIntervalMultipleSpiralShooter::BulletIntervalMultipleSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed, int shootCount, int interval)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRate(shootAngleRate), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_time(0)
	{

	}
	BulletIntervalMultipleSpiralShooter* BulletIntervalMultipleSpiralShooter::Get(const maths::vec3& position)
	{
		return new BulletIntervalMultipleSpiralShooter(position, 0, 0.02f, 1000, 4, 5);
	}
	void BulletIntervalMultipleSpiralShooter::Update()
	{
		if (m_time == 0)
		{
			for (int i = 0; i < m_shootCount; ++i)
				new Bullet(BULLET_TYPE::SPIRAL, m_position, m_shootAngle + static_cast<float>(i) / static_cast<float>(m_shootCount), 0, m_shootSpeed, 0);

			m_shootAngle += m_shootAngleRate;
			m_shootAngle -= floor(m_shootAngle);
		}

		m_time = (m_time + 1) % m_interval;
	}

	// ¹ÙÀÌ·² Á÷¼± ½ºÆÄÀÌ·²Åº
	BulletBiDirectionalSpiralShooter::BulletBiDirectionalSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate0, float shootAngleRate1, float shootSpeed, int shootCount, int interval)
		: BulletShooter(position), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_time(0)
	{
		m_shootAngle[0] = shootAngle;
		m_shootAngle[1] = shootAngle;
		m_shootAngleRate[0] = shootAngleRate0;
		m_shootAngleRate[1] = shootAngleRate1;
	}
	BulletBiDirectionalSpiralShooter* BulletBiDirectionalSpiralShooter::Get(const maths::vec3& position)
	{
		return new BulletBiDirectionalSpiralShooter(position, 0, 0.02f, -0.02f, 1000, 4, 5);

	}
	void BulletBiDirectionalSpiralShooter::Update()
	{
		if (m_time == 0)
		{
			for (int j = 0; j < 2; ++j)
			{
				for (int i = 0; i < m_shootCount; ++i)
				{
					new Bullet(BULLET_TYPE::SPIRAL, m_position, m_shootAngle[j] + static_cast<float>(i) / static_cast<float>(m_shootCount), 0, m_shootSpeed, 0);
				}

				m_shootAngle[j] += m_shootAngleRate[j];
				m_shootAngle[j] -= floor(m_shootAngle[j]);
			}
		}

		m_time = (m_time + 1) % m_interval;
	}

	// º¥Æ® ½ºÆÄÀÌ·²Åº
	BulletBentSpiralShooter::BulletBentSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed, int shootCount, int interval, float bulletAngleRate, float bulletSpeedRate)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRate(shootAngleRate), m_shootSpeed(shootSpeed), m_shootCount(shootCount), m_interval(interval), m_time(0), m_bulletAngleRate(bulletAngleRate), m_bulletSpeedRate(bulletSpeedRate)
	{

	}
	BulletBentSpiralShooter* BulletBentSpiralShooter::Get(const maths::vec3& position)
	{
		//return new BulletBentSpiralShooter(position, 90, 0, 1000.0f, 1, 10, -0.3f, 22.0f);
		return new BulletBentSpiralShooter(position, 0, 0.02f, 1000, 1, 10, -0.3, 0.2f);
	}
	void BulletBentSpiralShooter::Update()
	{
		if (m_time == 0)
		{
			for (int i = 0; i < m_shootCount; ++i)
				new Bullet(BULLET_TYPE::DIRECTIONAL, m_position, m_shootAngle, 0, m_shootSpeed, m_bulletSpeedRate);

			m_shootAngle += m_shootAngleRate;
			m_shootAngle -= floor(m_shootAngle);
		}

		m_time = (m_time + 1) % m_interval;
	}

	// ÄÄ¹ÙÀÎµå ½ºÆÄÀÌ·²Åº
	BulletCombinedSpiralShooter::BulletCombinedSpiralShooter(const maths::vec3& position)
		: BulletShooter(position)
	{
		m_bi = BulletBiDirectionalSpiralShooter::Get(position);
		m_bent = BulletBentSpiralShooter::Get(position);
	}
	BulletCombinedSpiralShooter* BulletCombinedSpiralShooter::Get(const maths::vec3& position)
	{
		return new BulletCombinedSpiralShooter(position);
	}
	void BulletCombinedSpiralShooter::Update()
	{
		m_bi->Update();
		m_bent->Update();
	}

}
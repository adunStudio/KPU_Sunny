#include "BulletShooter5.h"

namespace game
{
	// ÆÐÅÏÅº
	BulletPatternShooter* BulletPatternShooter::Get(const maths::vec3& position)
	{
		static char pattern[] =
			"                                   "
			"                                   "
			"                                   "
			"####  #   # #     #     ##### #####"
			"#   # #   # #     #     #       #  "
			"####  #   # #     #     ####    #  "
			"#   # #   # #     #     #       #  "
			"####  ##### ##### ##### #####   #  ";

		return new BulletPatternShooter(position, 0.25f, 0.3f, 1000, 5, pattern, 35, 8);
	}
	BulletPatternShooter::BulletPatternShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootSpeed, int interval, char* pattern, int width, int height)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRange(shootAngleRange), m_shootSpeed(shootSpeed), m_interval(interval), m_pattern(pattern), m_width(width), m_height(height)
	{

	}
	void BulletPatternShooter::Update()
	{
		if (m_time % m_interval == 0 && m_transformComponent)
		{
			char* p = m_pattern + (m_height - 1 - m_time / m_interval) * m_width;

			for (int i = m_width - 1; i >= 0; --i, ++p)
			{
				if (*p != ' ')
				{
					new Bullet(BULLET_TYPE::PATTERN, m_position, GetMyShipAngle(m_transformComponent->GetPosition(), m_position) + m_shootAngleRange * ((float)i / (m_width - 1) - 0.5f), 0, m_shootSpeed, 0);
				}
			}
		}

		m_time = (m_time + 1) % (m_interval * m_height);
	}


	// ½ºÅÜÅº
	BulletSteppingShooter* BulletSteppingShooter::Get(const maths::vec3& position)
	{
		return new BulletSteppingShooter(position, 0, 0.03f, 1000, 60, 20);
	}
	BulletSteppingShooter::BulletSteppingShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed, int moveTime, int stopTime)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRate(shootAngleRate), m_shootSpeed(shootSpeed), m_moveTime(moveTime), m_stopTime(stopTime)
	{

	}
	void BulletSteppingShooter::Update()
	{
		new SteppingBullet(BULLET_TYPE::STEP, m_position, m_shootAngle, m_shootSpeed, m_moveTime, m_stopTime);
		m_shootAngle += m_shootAngleRate;
		m_shootAngle -= floor(m_shootAngle);
	}

	// ½ºÇÃ¸´Åº
	BulletSplittingShooter* BulletSplittingShooter::Get(const maths::vec3& position)
	{
		return new BulletSplittingShooter(position, 0.25f, 0.4f, 7, 60);
	}
	BulletSplittingShooter::BulletSplittingShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootCount, int interval)
		: BulletShooter(position), m_shootAngle(shootAngle), m_shootAngleRange(shootAngleRange), m_shootCount(shootCount), m_interval(interval), m_time(0)
	{

	}
	void BulletSplittingShooter::Update()
	{
		if (m_time == 0 && m_transformComponent) 
		{
			if (m_shootCount > 1)
			{
				for (int i = 0; i < m_shootCount; ++i)
				{
					new SplittingBullet(BULLET_TYPE::STEP, m_position, m_shootAngle + m_shootAngleRange * ((float)i / (m_shootCount - 1) - 0.5f), 1000, 30, 5, 4, m_transformComponent);
				}
			}
		}

		m_time = (m_time + 1) % m_interval;
	}

	// º°¸ð¾çÅº
	BulletStarShooter* BulletStarShooter::Get(const maths::vec3& position)
	{
		return new BulletStarShooter(position, 0, 30, 100, 30, 2, 250, 1000, 0.4f);
	}
	BulletStarShooter::BulletStarShooter(const maths::vec3& position, float starAngle, float starSize, float shootTime, float waitTime, int interval, int cycle, float bulletSpeed, float bulletAngle)
		: BulletShooter(position), m_starAngle(starAngle), m_starSize(starSize), m_shootTime(shootTime), m_waitTime(waitTime), m_interval(interval), m_cycle(cycle), m_bulletSpeed(bulletSpeed), m_bulletAngle(bulletAngle), m_time(0)
	{

	}
	void BulletStarShooter::Update()
	{
		static const float x[]{  0, 0.59f * 20, -0.95f * 20,  0.95f * 20, -0.59f * 20,  0 };
		static const float z[]{ -1 * 20, 0.81f * 20, -0.31f * 20, -0.31f * 20,  0.81f * 20, -1 * 20 };

		int line_time = m_shootTime / 5;

		if (m_time < m_shootTime && m_time % m_interval == 0)
		{
			int line = m_time / line_time;
			int time = m_time % line_time;

			float ax = (x[line] + (x[line + 1] - x[line]) * m_time / (float)line_time);
			float az = (z[line] + (z[line + 1] - z[line]) * m_time / (float)line_time);

			float rad = m_starAngle * maths::SUNNY_PI * 2;
			float   c = cosf(rad);
			float   s = sinf(rad);
			float  bx = m_position.x + (ax * c - az * s) * m_starSize;
			float  bz = m_position.z + (ax * s + az * c) * m_starSize;

			maths::vec3 pos(bx, m_position.y, bz);

			new PlacedBullet(BULLET_TYPE::STAR_RED, pos, GetAngle(m_position.x, m_position.z, bx, bz) + m_bulletAngle, m_bulletSpeed, 0, m_shootTime + m_waitTime + m_time);
		}

		m_time = (m_time + 1) % m_cycle;
	}

}
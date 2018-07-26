#include "Bullet.h"

namespace game
{
	Bullet::Bullet(BULLET_TYPE type, const maths::vec3& position, float angle, float angleRate, float speed, float speedRate)
	: Pool(BossLocker::bulletList), type(type), alive(true), m_position(position), m_angle(angle), m_angleRate(angleRate), m_speed(speed), m_speedRate(speedRate)
	{

	}

	void* Bullet::operator new(size_t n)
	{
		return BossLocker::bulletList->New(n);
	}

	void Bullet::operator delete(void* p)
	{
		BossLocker::bulletList->Delete(p);
	}

	void Bullet::Update(float elapsedTime)
	{
		if (!alive) return;

		if (m_position.x < -MAX_X || m_position.x > MAX_X || m_position.z < -MAX_Z || m_position.x > MAX_Z)
			alive = false;

		float rad = m_angle * maths::SUNNY_PI * 2;
		
		//std::cout << m_speed * sinf(rad) * elapsedTime << std::endl;

		m_position.x += m_speed * cosf(rad) * elapsedTime;
		m_position.z -= m_speed * sinf(rad) * elapsedTime;

		m_angle += m_angleRate;
		m_speed += m_speedRate;
	}


	void SimpleHomingBullet::Update(float elapsedTime)
	{
		if (!m_transformComponent) return;

		m_angle = GetMyShipAngle(m_transformComponent->GetPosition(), m_position);
		Bullet::Update(elapsedTime);
	}

	void ConstrainedHomingBullet::Update(float elapsedTime)
	{
		if (!m_transformComponent) return;

		float angle_rate = GetMyShipAngle(m_transformComponent->GetPosition(), m_position) - m_angle;
		angle_rate -= floor(angle_rate);

		if (angle_rate <= m_maxAngleRate || 1 - angle_rate <= m_maxAngleRate)
			m_angle += angle_rate;
		else
			m_angle += (angle_rate < 0.5f) ? m_maxAngleRate : -m_maxAngleRate;

		m_angle -= floor(m_angle);

		Bullet::Update(elapsedTime);
	}

	void SteppingBullet::Update(float elapsedTime)
	{
		if (m_time == 0)
		{
			m_speed = m_initialSpeed;
		}
		else
		{
			if (m_time == m_moveTime)
			{
				m_speed = 0;
			}
		}


		m_time = (m_time + 1) % (m_moveTime + m_stopTime);
		
		Bullet::Update(elapsedTime);
	}

	void SplittingBullet::Update(float elapsedTime)
	{
		if (m_time == m_splitTime && m_transformComponent)
		{
			m_shootAngle = GetMyShipAngle(m_transformComponent->GetPosition(), m_position);
		}

		if (m_time < m_splitTime)
		{
			Bullet::Update(elapsedTime);
		}
		else
		{
			if (m_time < m_splitTime + m_splitCount * m_splitInterval)
			{
				if (((m_time - m_splitTime) % m_splitInterval) == 0)
				{
					new Bullet(BULLET_TYPE::DIRECTIONAL, m_position, m_shootAngle, 0, m_speed, 0);
				}
			}
			else
			{
				alive = false;
			}
		}

		m_time++;
	}

	void PlacedBullet::Update(float elapsedTime)
	{
		if (m_time == m_moveTime)
			m_speed = 0;

		if (m_time == m_moveTime + m_stopTime)
			m_speed = m_initialSpeed;

		m_time++;

		Bullet::Update(elapsedTime);
	}

}
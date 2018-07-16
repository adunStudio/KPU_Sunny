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


}
#include "Bullet.h"

namespace game
{
	Bullet::Bullet(BULLET_TYPE type, const maths::vec3& position, float angle, float angleRate, float speed, float speedRate)
	: type(type), alive(true), m_angle(angle), m_angleRate(angleRate), m_speed(speed), m_speedRate(speedRate)
	{

	}

	Bullet::~Bullet()
	{

	}

	void Bullet::Update(float elapsedTime)
	{
		if (!alive) return;

		if (m_position.x < -MAX_X || m_position.x > MAX_X || m_position.z < -MAX_Z || m_position.x > MAX_Z)
			alive = false;

		float rad = m_angle * maths::SUNNY_PI * 2;
		float xMove = m_speed * cosf(rad) * elapsedTime;
		float zMove = m_speed * sinf(rad) * elapsedTime;

		m_position.x += xMove;
		m_position.z -= xMove;

		m_angle += m_angleRate * elapsedTime;
		m_speed += m_speedRate * elapsedTime;
	}


}
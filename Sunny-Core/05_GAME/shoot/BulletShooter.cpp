#include "BulletShooter.h"

namespace game
{
	// Á÷¼±Åº
	BulletDirectionalShooter::BulletDirectionalShooter(const maths::vec3& position, float shootAngle, float shootSpeed)
	: BulletShooter(position), m_shootAngle(shootAngle), m_shootSpeed(shootSpeed)
	{

	}
	BulletDirectionalShooter* BulletDirectionalShooter::Get1(const maths::vec3& position)
	{
		return new BulletDirectionalShooter(position, 0.25f, 500);
	}
	BulletDirectionalShooter* BulletDirectionalShooter::Get2(const maths::vec3& position)
	{
		return new BulletDirectionalShooter(position, 0.5f, 500);
	}
	BulletDirectionalShooter* BulletDirectionalShooter::Get3(const maths::vec3& position)
	{
		return new BulletDirectionalShooter(position, 0.375f, 500);
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
	BulletSpiralShooter* BulletSpiralShooter::Get1(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.02f, 500);
	}
	BulletSpiralShooter* BulletSpiralShooter::Get2(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.01f, 500);
	}
	BulletSpiralShooter* BulletSpiralShooter::Get3(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.03f, 500);
	}
	BulletSpiralShooter* BulletSpiralShooter::Get4(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.02f, 260);
	}
	BulletSpiralShooter* BulletSpiralShooter::Get5(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.02f, 1000);
	}
	void BulletSpiralShooter::Update()
	{
		new Bullet(BULLET_TYPE::SPIRAL, m_position, m_shootAngle, 0, m_shootSpeed, 0);
		m_shootAngle += m_shootAngleRate;
		m_shootAngle -= floor(m_shootAngle);
	}
}
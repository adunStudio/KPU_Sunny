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
		return new BulletDirectionalShooter(position, 0.25f, 0.1f);
	}
	BulletDirectionalShooter* BulletDirectionalShooter::Get2(const maths::vec3& position)
	{
		return new BulletDirectionalShooter(position, 0.5f, 0.1f);
	}
	BulletDirectionalShooter* BulletDirectionalShooter::Get3(const maths::vec3& position)
	{
		return new BulletDirectionalShooter(position, 0.375f, 0.1f);
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
		return new BulletSpiralShooter(position, 0, 0.02f, 0.01f);
	}
	BulletSpiralShooter* BulletSpiralShooter::Get2(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.01f, 0.01f);
	}
	BulletSpiralShooter* BulletSpiralShooter::Get3(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.03f, 0.01f);
	}
	BulletSpiralShooter* BulletSpiralShooter::Get4(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.02f, 0.005f);
	}
	BulletSpiralShooter* BulletSpiralShooter::Get5(const maths::vec3& position)
	{
		return new BulletSpiralShooter(position, 0, 0.02f, 0.02f);
	}
	void BulletSpiralShooter::Update()
	{
		new Bullet(BULLET_TYPE::SPIRAL, m_position, m_shootAngle, 0, m_shootSpeed, 0);
		m_shootAngle += m_shootAngleRate;
		m_shootAngle -= floor(m_shootAngle);
	}
}
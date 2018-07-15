#pragma once
#include <SUNNY.h>
#include "Bullet.h"

namespace game
{
	class BulletShooter
	{
	protected:
		maths::vec3 m_position;

	public:
		BulletShooter(const maths::vec3& position) : m_position(position) { };

		virtual void Update() = 0;
	};

	// Á÷¼±Åº
	class BulletDirectionalShooter : BulletShooter
	{
	public:
		static BulletDirectionalShooter* Get1(const maths::vec3& position);
		static BulletDirectionalShooter* Get2(const maths::vec3& position);
		static BulletDirectionalShooter* Get3(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootSpeed;

	private:
		BulletDirectionalShooter(const maths::vec3& position, float shootAngle, float shootSpeed);

	public:
		void Update() override;
	};

	// ½ºÆÄÀÌ·²Åº
	class BulletSpiralShooter : BulletShooter
	{
	public:
		static BulletSpiralShooter* Get1(const maths::vec3& position);
		static BulletSpiralShooter* Get2(const maths::vec3& position);
		static BulletSpiralShooter* Get3(const maths::vec3& position);
		static BulletSpiralShooter* Get4(const maths::vec3& position);
		static BulletSpiralShooter* Get5(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRate;
		float m_shootSpeed;
	
	private:
		BulletSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed);

	public:
		void Update() override;
	};


}
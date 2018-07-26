#pragma once

#include "Bullet.h"

namespace game
{
	// ½ÉÇÃÈ¨À×Åº
	class BulletSimpleHomingShooter : public BulletShooter
	{
	public:
		static BulletSimpleHomingShooter* Get(const maths::vec3& position);

	protected:
		float m_shootSpeed;
		int   m_interval;
		int   m_time;

	protected:
		BulletSimpleHomingShooter(const maths::vec3& position, float shootSpeed, int interval);

	public:
		void Update() override;
	};

	// ÄÁ½ºÆ®·»ÀÎµåÈ¨À×Åº
	class BulletConstrainedHomingShooter : public BulletSimpleHomingShooter
	{
	public:
		static BulletConstrainedHomingShooter* Get(const maths::vec3& position);

	private:
		float m_maxAngleRate;

	private:
		BulletConstrainedHomingShooter(const maths::vec3& position, float shootSpeed, int interval, float maxAngleRate);

	public:
		void Update() override;
	};

	// °¸Åº
	class BulletGapShooter : public BulletShooter
	{
	public:
		static BulletGapShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRange;
		float m_shootSpeed;
		int   m_shootCount;
		int   m_interval;
		int   m_time;
		float m_bulletAngleRate;
		float m_bulletSpeedRate;

	protected:
		BulletGapShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootSpeed, int shootCount, int interval, float bulletAngleRate, float bulletSpeedRate);

	public:
		void Update() override;
	};
}


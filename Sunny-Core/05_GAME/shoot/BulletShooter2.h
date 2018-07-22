#pragma once
#include <SUNNY.h>
#include "Bullet.h"

namespace game
{
	// NWay탄
	class BulletNWayShooter : public BulletShooter
	{
	public:
		static BulletNWayShooter* Get(const maths::vec3& position);

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
		BulletNWayShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootSpeed, int shootCount, int interval, float bulletAngleRate, float bulletSpeedRate);

	public:
		void Update() override;
	};

	// Circle NWay탄
	class BulletCircleNWayShooter : public BulletNWayShooter
	{
	public:
		static BulletCircleNWayShooter* Get(const maths::vec3& position);

	private:
		BulletCircleNWayShooter(const maths::vec3& position, float shootAngle, float shootSpeed, float shootCount, int interval, float bulletAngleRate, float bulletSpeedRate);
	};

	// Bent Circle NWay탄
	class BulletBentCircleNWayShooter : public BulletNWayShooter
	{
	public:
		static BulletBentCircleNWayShooter* Get(const maths::vec3& position);

	private:
		BulletBentCircleNWayShooter(const maths::vec3& position, float shootAngle, float shootSpeed, float shootCount, int interval, float bulletAngleRate, float bulletSpeedRate);
	};

	// Aiming NWay탄
	class BulletAimingNWayShooter : public BulletShooter
	{
	public:
		static BulletAimingNWayShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngleRange;
		float m_shootSpeed;
		int   m_shootCount;
		int   m_interval;
		int   m_time;

	private:
		BulletAimingNWayShooter(const maths::vec3& position, float shootAngleRange, float shootSpeed, int shootCount, int interval);

	public:
		void Update() override;
	};

	// Intermittent Aiming NWay탄
	class BulletIntermittentAimingNWayShooter : public BulletShooter
	{
	public:
		static BulletIntermittentAimingNWayShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngleRange;
		float m_shootSpeed;
		int   m_shootCount;
		int   m_interval;
		int   m_time;
		int   m_shootTime;
		int   m_waitTime;

	private:
		BulletIntermittentAimingNWayShooter(const maths::vec3& position, float shootAngleRange, float shootSpeed, int shootCount, int interval, int shootTime, int waitTime);

	public:
		void Update() override;
	};

	// Random Nway탄
	class BulletRandomNWayShooter : public BulletShooter
	{
	public:
		static BulletRandomNWayShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRange;
		float m_shootSpeed;
		int   m_shootCount;
		int   m_interval;
		int   m_time;

	private:
		BulletRandomNWayShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootSpeed, int shootCount, int interval);

	public:
		void Update() override;
	};
}
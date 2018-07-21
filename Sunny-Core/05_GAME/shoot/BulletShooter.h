#pragma once
#include <SUNNY.h>
#include "Bullet.h"

namespace game
{
	// Á÷¼±Åº
	class BulletDirectionalShooter : public BulletShooter
	{
	public:
		static BulletDirectionalShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootSpeed;

	private:
		BulletDirectionalShooter(const maths::vec3& position, float shootAngle, float shootSpeed);

	public:
		void Update() override;
	};

	// ½ºÆÄÀÌ·²Åº
	class BulletSpiralShooter : public BulletShooter
	{
	public:
		static BulletSpiralShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRate;
		float m_shootSpeed;
	
	private:
		BulletSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed);

	public:
		void Update() override;
	};

	// ¸ÖÆ¼ ½ºÆÄÀÌ·²Åº
	class BulletMultipleSpiralShooter : public BulletShooter
	{
	public:
		static BulletMultipleSpiralShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRate;
		float m_shootSpeed;
		int   m_shootCount;

	private:
		BulletMultipleSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed, int shootCount);

	public:
		void Update() override;
	};

	// ÀÎÅÍ¹ú ¸ÖÆ¼ ½ºÆÄÀÌ·²Åº
	class BulletIntervalMultipleSpiralShooter : public BulletShooter
	{
	public:
		static BulletIntervalMultipleSpiralShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRate;
		float m_shootSpeed;
		int   m_shootCount;
		int   m_interval;
		int   m_time;

	private:
		BulletIntervalMultipleSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed, int shootCount, int interval);

	public:
		void Update() override;
	};

	// ¹ÙÀÌ·² Á÷¼± ½ºÆÄÀÌ·²Åº
	class BulletBiDirectionalSpiralShooter : public BulletShooter
	{
	public:
		static BulletBiDirectionalSpiralShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle[2];
		float m_shootAngleRate[2];
		float m_shootSpeed;
		int   m_shootCount;
		int   m_interval;
		int   m_time;

	private:
		BulletBiDirectionalSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate0, float shootAngleRate1, float shootSpeed, int shootCount, int interval);

	public:
		void Update() override;
	};

	// º¥Æ® ½ºÆÄÀÌ·²Åº
	class BulletBentSpiralShooter : public BulletShooter
	{
	public:
		static BulletBentSpiralShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRate;
		float m_shootSpeed;
		int   m_shootCount;
		int   m_interval;
		int   m_time;
		float m_bulletAngleRate;
		float m_bulletSpeedRate;

	private:
		BulletBentSpiralShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed, int shootCount, int interval, float bulletAngleRate, float bulletSpeedRate);

	public:
		void Update() override;
	};

	// ÄÄ¹ÙÀÎµå ½ºÆÄÀÌ·²Åº
	class BulletCombinedSpiralShooter : public BulletShooter
	{
	public:
		static BulletCombinedSpiralShooter* Get(const maths::vec3& position);

	private:
		BulletBiDirectionalSpiralShooter* m_bi;
		BulletBentSpiralShooter* m_bent;

	private:
		BulletCombinedSpiralShooter(const maths::vec3& position);

	public:
		void Update() override;
	};
}
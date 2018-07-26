#pragma once
#include "Bullet.h"

namespace game
{
	// ∆–≈œ≈∫
	class BulletPatternShooter : public BulletShooter
	{
	public:
		static BulletPatternShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRange;
		float m_shootSpeed;
		int   m_interval;
		char* m_pattern;
		int   m_width;
		int   m_height;
		int   m_time;

	private:
		BulletPatternShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootSpeed, int interval, char* pattern, int width, int height);

	public:
		void Update() override;
	};

	// Ω∫≈‹≈∫
	class BulletSteppingShooter : public BulletShooter
	{
	public:
		static BulletSteppingShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRate;
		float m_shootSpeed;
		int   m_moveTime;
		int   m_stopTime;

	private:
		BulletSteppingShooter(const maths::vec3& position, float shootAngle, float shootAngleRate, float shootSpeed, int moveTime, int stopTime);
	
	public:
		void Update() override;
	};

	// Ω∫«√∏¥≈∫
	class BulletSplittingShooter : public BulletShooter
	{
	public:
		static BulletSplittingShooter* Get(const maths::vec3& position);

	private:
		float m_shootAngle;
		float m_shootAngleRange;
		float m_shootCount;
		int   m_interval;
		int   m_time;

	private:
		BulletSplittingShooter(const maths::vec3& position, float shootAngle, float shootAngleRange, float shootCount, int interval);

	public:
		void Update() override;
	};

	// ∫∞∏æÁ≈∫
	class BulletStarShooter : public BulletShooter
	{
	public:
		static BulletStarShooter* Get(const maths::vec3& position);

	private:
		float m_starAngle;
		float m_starSize;
		float m_shootTime;
		float m_waitTime;
		int   m_interval;
		int   m_cycle;
		int   m_time;
		float m_bulletSpeed;
		float m_bulletAngle;

		inline float GetAngle(float x, float y, float to_x, float to_y)
		{
			return atan2f(to_y - y, to_x - x) / maths::SUNNY_PI / 2;
		}

	private:
		BulletStarShooter(const maths::vec3& position, float starAngle, float starSize, float shootTime, float waitTime, int interval, int cycle, float bulletSpeed, float bulletAngle);

	public:
		void Update() override;
	};
}

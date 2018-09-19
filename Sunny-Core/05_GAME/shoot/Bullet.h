#pragma once
#include <SUNNY.h>
#include "../07_SERVER/BossLocker.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;
using namespace maths;

namespace game
{
	enum BULLET_TYPE
	{
		DIRECTIONAL,
		SPIRAL,
		NWAY,
		AIMING,
		LINE,
		PATTERN,
		STEP,
		STAR_RED,
		STAR_BLUE
	};

	// ±âº»Åº
	class Bullet : public Pool
	{
	protected:
		const int MAX_X = 1500;
		const int MAX_Z = 1500;

	protected:
		maths::vec3 m_position;
		
		float m_speed;
		float m_angle;

		float m_speedRate;
		float m_angleRate;
		
		float m_totalMovement;

	public:
		bool alive;
		bool isCon = false;

		BULLET_TYPE type;

	public:
		Bullet(BULLET_TYPE type, const maths::vec3& position, float angle, float angleRate, float speed, float speedRate);

		void* operator new(size_t n);

		void operator delete(void* p);

		virtual void Update(float elapsedTime);

		float GetMyShipAngle(maths::vec3& enemy, maths::vec3& shooter)
		{
			return atan2f(enemy.z - shooter.z, enemy.x - shooter.x) / maths::SUNNY_PI / 2 * -1.0f;
		}

	public:
		inline const maths::vec3& GetPosition() const { return m_position; }
	};

	// ½ÉÇÃÈ¨À×Åº
	class SimpleHomingBullet : public Bullet
	{
	private:
		TransformComponent* m_transformComponent;

	public:
		SimpleHomingBullet(BULLET_TYPE type, const maths::vec3& position, float angle, float speed, TransformComponent* transformComponent)
			: Bullet(type, position, angle, 0, speed, 0), m_transformComponent(transformComponent)
		{
	
		}

		virtual void Update(float elapsedTime);
	};

	// ÄÁ½ºÆ®·¹ÀÎµåÈ¨À×Åº
	class ConstrainedHomingBullet : public Bullet
	{
	private:
		TransformComponent* m_transformComponent;
		float m_maxAngleRate;

	public:
		ConstrainedHomingBullet(BULLET_TYPE type, const maths::vec3& position, float angle, float speed, float maxAngleRate, TransformComponent* transformComponent)
			: Bullet(type, position, angle, 0, speed, 0), m_maxAngleRate(maxAngleRate), m_transformComponent(transformComponent)
		{

		}

		virtual void Update(float elapsedTime);
	};

	// ½ºÅÜÅº
	class SteppingBullet : public Bullet
	{
	private:
		float m_initialSpeed;
		int   m_moveTime;
		int   m_stopTime;
		int   m_time;

	public:
		SteppingBullet(BULLET_TYPE type, const maths::vec3& position, float angle, float speed, int moveTime, int stopTime)
			: Bullet(type, position, angle, 0, speed, 0), m_initialSpeed(speed), m_moveTime(moveTime), m_stopTime(stopTime), m_time(0)
		{

		}

		virtual void Update(float elapsedTime);
	};

	// ½ºÇÃ¸´Åº
	class SplittingBullet : public Bullet
	{
	private:
		int m_splitTime;
		int m_splitCount;
		int m_splitInterval;
		int m_time;
		float m_shootAngle;
		TransformComponent* m_transformComponent;

	public:
		SplittingBullet(BULLET_TYPE type, const maths::vec3& position, float angle, float speed, int splitTime, int splitCount, int splitInterval, TransformComponent* transformComponent)
			: Bullet(type, position, angle, 0, speed, 0), m_splitTime(splitTime), m_splitCount(splitCount), m_splitInterval(splitInterval), m_time(0), m_transformComponent(transformComponent)
		{
		}

		virtual void Update(float elapsedTime);
	};

	// ÇÃ·¹ÀÌ½ºÅº
	class PlacedBullet : public Bullet
	{
	private:
		float m_initialSpeed;
		int   m_moveTime;
		int   m_stopTime;
		int   m_time;

	public:
		PlacedBullet(BULLET_TYPE type, const maths::vec3& position, float angle, float speed, int moveTime, int stopTime)
			: Bullet(type, position, angle, 0, speed, 0), m_initialSpeed(speed), m_moveTime(moveTime), m_stopTime(stopTime), m_time(0)
		{
		}

		virtual void Update(float elapsedTime);
	};

	// ÃÑ¾Ë»ý¼º±â
	class BulletShooter
	{
	protected:
		maths::vec3 m_position;
		TransformComponent* m_transformComponent;

	protected:
		float GetMyShipAngle(maths::vec3& enemy, maths::vec3& shooter) {
			return atan2f(enemy.z - shooter.z, enemy.x - shooter.x) / maths::SUNNY_PI / 2 * -1.0f;
		}

	public:
		BulletShooter(const maths::vec3& position) : m_position(position), m_transformComponent(nullptr) { };

		inline void SetEnemyTransform(TransformComponent* transformComponent) { m_transformComponent = transformComponent; }

		virtual void Update() { };
	};
}
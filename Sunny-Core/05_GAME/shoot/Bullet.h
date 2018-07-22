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
		NWAY
	};

	class Bullet : public Pool
	{
	private:
		const int MAX_X = 1500;
		const int MAX_Z = 1500;

	private:
		maths::vec3 m_position;
		
		float m_speed;
		float m_angle;

		float m_speedRate;
		float m_angleRate;
		
		float m_totalMovement;

	public:
		bool alive;

		BULLET_TYPE type;

	public:
		Bullet(BULLET_TYPE type, const maths::vec3& position, float angle, float angleRate, float speed, float speedRate);

		void* operator new(size_t n);

		void operator delete(void* p);

		virtual void Update(float elapsedTime);

	public:
		inline const maths::vec3& GetPosition() const { return m_position; }
	};

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
#pragma once

#include "Bullet.h"

// À¯µµÅº
class BulletAimingShooter : public BulletShooter
{
public:
	static BulletAimingShooter* Get(const maths::vec3& position);

private:
	float m_shootSpeed;
	int   m_interval;
	int   m_time;

private:
	BulletAimingShooter(const maths::vec3& position, float shootSpeed, int interval);

public:
	void Update() override;
};

// ¶óÀÎÅº
class BulletLineShooter : public BulletShooter
{
public:
	static BulletLineShooter* Get(const maths::vec3& position);

private:
	float m_shootAngle;
	float m_shootSpeed;
	int   m_interval;
	int   m_time;
	int   m_shootTime;
	int   m_waitTime;

private:
	BulletLineShooter(const maths::vec3& position, float shootAngle, float shootSpeed, int interval, int shootTime, int waitTime);

public:
	void Update() override;
};

// ½ºÇÁ·¹µùÅº
class BulletSpreadingShooter : public BulletShooter
{
public:
	static BulletSpreadingShooter* Get(const maths::vec3& position);

private:
	float m_shootAngleRange;
	float m_shootSpeed;
	int   m_shootCount;
	int   m_interval;
	int   m_time;
	float m_groupSpeed;
	int   m_groupCount;

private:
	BulletSpreadingShooter(const vec3& position, float shootAngleRange, float shootSpeed, int shootCount, int interval, float groupSpeed, int groupCount);

public:
	void Update() override;
};

// ·£´ý ½ºÇÁ·¹µùÅº
class BulletRandomSpreadingShooter : public BulletShooter
{
public:
	static BulletRandomSpreadingShooter* Get(const maths::vec3& position);

private:
	float m_shootAngleRange;
	float m_shootSpeed;
	float m_shootSpeedRange;
	int   m_shootCount;
	int   m_interval;
	int   m_time;

private:
	BulletRandomSpreadingShooter(const maths::vec3& position, float shootAngleRange, float shootSpeed, float shootSpeedRange, int shootCount, int interval);

public:
	void Update() override;
};

// ¿À¹öÅ×ÀÌÅ·Åº
class BulletOvertakingShooter : public BulletShooter
{
public:
	static BulletOvertakingShooter* Get(const maths::vec3& position);

private:
	float m_shootAngleRange;
	float m_shootSpeed;
	int   m_shootCount;
	int   m_interval;
	float m_groupSpeed;
	float m_groupAngle;
	int   m_groupCount;
	int   m_groupInterval;
	float m_shootAngle;
	int   m_time;

private:
	BulletOvertakingShooter(const maths::vec3& position, float shootAngleRange, float shootSpeed, float shootCount, int interval, int groupSpeed, int groupAngle, int groupCount, int groupInterval);

public:
	void Update() override;
};
#pragma once
#include"../05_GAME/shoot/CBullet.h"

#include<vector>
#include<random>


using namespace std;

static char pattern[] =
"                                                               "
"                                                               "
"########   ########   ########   ########   ########      #    "
"#      #   #      #   #          #          #             #    "
"#      #   #      #   #          #          #             #    "
"########   #      #   #          #          ########      #    "
"#      #   #      #   #          #          #             #    "
"#      #   #      #   #          #          #             #    "
"########   #      #   #          #          ########   ########";

static char starPattern[] =
"                                                               "
"                                                               "
"       #           "
"   #########             "
"    #      #        "
"      #   #         "
"        #          "
"      #   #        "
"     #      #       ";

class BulletSystem
{
public:
	BulletSystem(Model* mesh, Layer3D* layer);
	~BulletSystem();
	virtual void Update(float elapsedTime) = 0;
	virtual void SetTarget(float x, float y) {}
	void SetAngle(float angle) { m_angle = angle; }
	void SetPosition(const float x,const float y,const float z) { m_startPosition.x = x; m_startPosition.y = y; m_startPosition.z = z;}
	
	Point GetPosition() const{ return m_startPosition; }
protected:
	Point m_startPosition = { 0,0,0 };
	Model* m_mesh=nullptr;
	Layer3D* m_layer=nullptr;
	float m_angle = { 0.25 };
public:
	vector<CBullet*> bullets;
};




class AmingSystem : public BulletSystem
{
	using BulletSystem::BulletSystem;
	virtual void SetTarget(float x, float y) { m_angle = atan2f(y -m_startPosition.y, x - m_startPosition.x) / 3.1415 / 2; };

};


class BiDirectionalSystem : public BulletSystem
{
public:
	using BulletSystem::BulletSystem;
	void Update(float elapsedTime);
public:
	float m_angle[2] = {0,0};
	float m_shotAnglerate[2] = {0.03f,-0.02f};
	float m_firerate = 0;
	float m_speed = 8;
	int multiShotCount = 4;
};


class BentSystem : public BulletSystem
{
public:
	using BulletSystem::BulletSystem;
	void Update(float elapsedTime);
public:
	float m_angle = {0 };
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration=-0.03f;
	float m_speedAcceleration=1.f;
	float m_firerate = 0;
	float m_speed = 3;
	int multiShotCount = 4;
};

class BulletGenerator3 :public BulletSystem
{
public:
	using BulletSystem::BulletSystem;
	void Update(float elapsedTime);
public:
	float m_angle = { 0 };
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange=1;
	int multiShotCount = 25;
};

class AimingNWAYSystem :public AmingSystem
{
public:
	using AmingSystem::AmingSystem;
	void Update(float elapsedTime);
public:
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange = 0.25;
	float m_totalTime = 0;
	int multiShotCount = 7;
};

class BulletGenerator9 :public AmingSystem// π´¿€¿ß ≈∫
{
public:
	using AmingSystem::AmingSystem;
	void Update(float elapsedTime);
public:
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange = 0.25;
	float m_coolTime = 0;
	float m_totalTime = 0;
	int multiShotCount = 5;

};



class BulletGenerator2 :public AmingSystem //∆–≈œ≈∫
{
public:
	using AmingSystem::AmingSystem;
	void Update(float elapsedTime);
public:
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 1;
	float m_angleRange = 0.25;
	int multiShotCount = 5;

	int width = 63;
	int height = 9;
	float m_totalTime = 0;
};
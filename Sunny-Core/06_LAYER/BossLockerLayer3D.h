#pragma once

#include <iostream>
#include <SUNNY.h>

#include "../06_LAYER/BossLockerLayer2D.h"
#include "../05_GAME/graphics/Model3D.h"
#include "../05_GAME/graphics/Animation3D.h"
#include "../05_GAME/graphics/BulletM.h"
#include "../07_SERVER/BossLockerProtocol.h"
#include "../07_SERVER/BossLocker.h"

#include "../05_GAME/shoot/BulletShooter.h"
#include "../05_GAME/shoot/BulletShooter2.h"
#include "../05_GAME/shoot/BulletShooter3.h"
#include "../05_GAME/shoot/BulletShooter4.h"
#include "../05_GAME/shoot/BulletShooter5.h"
#include "../05_GAME/shoot/BulletParticle.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;
using namespace game;
using namespace maths;


class BossLockerLayer3D : public Layer3D
{
public:
	unordered_map<int, BulletShooter*> m_shooters;

private:
	BossLockerLayer2D * m_layer2D;
	SOCKET m_socket;

	ParticleSystem* m_bulletParticle;

	unordered_map<int, vector<Mesh*>> m_animations;

	Entity * m_entity;
	Entity*  m_boss;

	MaterialInstance* m_skyboxMaterial;

	vector<Model3D*> m_mapObjects;


	chrono::steady_clock::time_point start;

	float m_radian;
	float m_degree;

public:
	WSABUF m_send_wsabuf;
	char   m_send_buffer[MAX_BUFF_SIZE];
	unsigned long m_io_flag = 0;

public:
	BossLockerLayer3D();
	~BossLockerLayer3D();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;

	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnKeyReleasedEvent(KeyReleasedEvent& event);
	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);
	bool OnMouseMovedEvent(MouseMovedEvent& event);
};
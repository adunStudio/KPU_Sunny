#pragma once

#include <iostream>
#include <SUNNY.h>

#include "../06_LAYER/TestGameLayer2D.h"
#include "../05_GAME/graphics/Model3D.h"
#include "../05_GAME/graphics/Animation3D.h"
#include "../05_GAME/graphics/Bullet.h"
#include "../07_SERVER/BossLockerProtocol.h"
#include "../07_SERVER/BossLocker.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;
using namespace game;
using namespace maths;


class TestGameLayer3D : public Layer3D
{
public:
	int m_windowWidth, m_windowHeight;

	TestGameLayer2D* m_layer2D;

	unordered_map<int, vector<Mesh*>> m_animations;

	Entity * m_entity;
	Model* m_sphere;
	Entity* m_boss;

	MaterialInstance* m_SkyboxMaterial;

	vector<Model3D*> m_mapObjects;

	float m_radian;
	float m_degree;

public:
	WSABUF m_send_wsabuf;
	char   m_send_buffer[MAX_BUFF_SIZE];
	unsigned long m_io_flag = 0;

public:
	TestGameLayer3D();
	~TestGameLayer3D();

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
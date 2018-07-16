#pragma once

#include <iostream>
#include <SUNNY.h>

#include "../05_GAME/graphics/Model3D.h"
#include "../05_GAME/graphics/Animation3D.h"
#include "../05_GAME/graphics/BulletM.h"
#include "../05_GAME/MousePicker.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;
using namespace game;
using namespace maths;

class TestLayer2D;


class TestLayer3D : public Layer3D
{
public:
	Entity* m_entity;
	Animation3D* m_character;
	vector<BulletM	*> m_bullets;
	Model* m_sphere;

	MaterialInstance* m_SkyboxMaterial;

	MousePicker* m_mousePicker;

	vector<Model3D*> m_mapObjects;


	Model3D* m_pickedModel = nullptr;

	float m_radian = 0;
	float m_degree = 0;

public:
	TestLayer3D();
	~TestLayer3D();

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
#pragma once

#include <iostream>
#include <SUNNY.h>

#include "../05_GAME/graphics/Model3D.h"
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
	MaterialInstance* m_SkyboxMaterial;

	MousePicker* m_mousePicker;

	vector<Model3D*> m_mapObjects;

	TestLayer2D& m_testLayer2D;

	Model3D* m_pickedModel = nullptr;

public:
	TestLayer3D(TestLayer2D& layer);
	~TestLayer3D();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;

	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnKeyReleasedEvent(KeyReleasedEvent& event);
	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);
};
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

class TestLayer3D : public Layer3D
{
public:
	Entity* m_entity;
	MaterialInstance* m_SkyboxMaterial;

	MousePicker* m_mousePicker;

public:
	TestLayer3D();
	~TestLayer3D();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;

	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnMousePressedEvent(MousePressedEvent& event);
};
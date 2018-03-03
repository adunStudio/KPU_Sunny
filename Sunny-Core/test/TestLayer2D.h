#pragma once

#include <iostream>
#include "../graphics/layers/Layer2D.h"
#include "../app/Application.h"
#include "../ui/Panel.h"
#include "../ui/Button.h"
#include "../ui/Slider.h"

#include "NPCLayer.h"
#include "BossLayer.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;
using namespace ui;

class TestLayer2D : public Layer2D
{
private:
	Sprite* m_logo;

	Label*  m_fps;

	Panel* m_panel;

	std::vector<Layer*> m_layers;
	
	int m_layerIndex;

public:
	TestLayer2D();
	~TestLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);

	void ButtonEvent1();
};
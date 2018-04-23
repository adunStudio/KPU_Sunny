#pragma once

#include <iostream>
#include <SUNNY.h>

#include "NPCLayer.h"
#include "BossLayer.h"
#include "TestLayer3D.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;
using namespace ui;

class TestLayer2D : public Layer2D
{
public:
	TestLayer3D* m_testLayer3D;

	Sprite* m_logo;

	Label*  m_fps;

	Panel* m_model_panel;
	Label* model_position;
	Label* model_rotation;
	Label* model_scale;


	Panel* m_panel;
	
	std::vector<Layer*> m_layers;
	
	int m_layerIndex;

	Slider* m_slider1;
	Slider* m_slider2;

	Progressbar* m_progressbar;

public:
	TestLayer2D();
	~TestLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);

	void SaveEvent();
};
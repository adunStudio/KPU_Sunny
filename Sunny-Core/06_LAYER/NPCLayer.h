#pragma once

#include <iostream>
#include <SUNNY.h>

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;

class NPCLayer : public Layer3D
{
public:
	Entity* m_entity;

public:
	NPCLayer();
	~NPCLayer();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;

	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
};
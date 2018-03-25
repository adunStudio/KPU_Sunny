#pragma once
#pragma once

#include <iostream>
#include "../graphics/layers/Layer3D.h"
#include "../app/Application.h"
#include "../graphics/shaders/ShaderFactory.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;

class BossLayer : public Layer3D
{
public:
	Entity* m_entity;

	HeightMap* m_heightMap;

private:
	std::vector<Entity*> entities;

public:
	BossLayer();
	~BossLayer();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;

	void SetEntity(unsigned int index);

	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
};
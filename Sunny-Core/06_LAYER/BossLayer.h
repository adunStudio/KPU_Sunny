#pragma once
#pragma once

#include <iostream>
#include <SUNNY.h>

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;

class BossLayer : public Layer3D
{
public:
	Entity* m_entity;
	
	Group3D* m_group;
	
	HeightMap* m_heightMap;

	Terrain* m_terrain;

	std::map<std::string, Model*> m_models;

	std::vector<Entity*> m_characters;

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
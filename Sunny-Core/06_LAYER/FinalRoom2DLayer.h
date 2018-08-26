#pragma once

#include <iostream>
#include <SUNNY.h>
#include "FinalGame.h"
#include "../07_SERVER/BossLockerProtocol.h"
#include "../06_LAYER/ParticleLayer3D.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;
using namespace game;

class FinalRoom2DLayer : public Layer2D
{
public:
	Panel * m_panel;

	unordered_map<string, Sprite*> m_sprites;
	unordered_map<string, Button*> m_buttons;

	int m_mapSelect = 0;

private:
	bool firstTime;

public:
	FinalRoom2DLayer();
	~FinalRoom2DLayer();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);

	void ButtonClick();

	void SelectMap(int index);
	void SelectCharacter(int index);
};
#pragma once

#include <iostream>
#include <SUNNY.h>
#include "../05_GAME/Player.h"
#include "../07_SERVER/BossLocker.h"
#include "../07_SERVER/BossLockerProtocol.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;
using namespace ui;
using namespace game;

class FinalGameLayer2D : public Layer2D
{
public:
	Panel * m_panel;

	Progressbar* m_bossHPBar;
	Progressbar* m_playerHPBar;


public:
	Animation3D* m_boss;
	Animation3D* m_player;
	

public:
	FinalGameLayer2D(Animation3D* boss, Animation3D* player);
	~FinalGameLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnMouseMovedEvent(MouseMovedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);
	bool OnKeyPressedEvent(KeyPressedEvent& event);
};
#pragma once

#include <iostream>
#include <SUNNY.h>
#include "../05_GAME/Player.h"
#include "../07_SERVER/BossLocker.h"
#include "../07_SERVER/BossLockerProtocol.h"
#include "../05_GAME/shoot/BulletShooter.h"
#include "../05_GAME/graphics/CharacterHpBar.h"
#include "GameOverLayer2D.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;
using namespace ui;
using namespace game;

class BossLockerLayer3D;

class BossLockerLayer2D : public Layer2D
{
public:
	BossLockerLayer3D * m_layer3D;

	Panel* m_panel;

	Progressbar* m_bossHPBar;

	Sprite* m_test;

	std::vector<CharacterHpBar*> characterHpBars;
	

public:
	WSABUF m_send_wsabuf;
	char   m_send_buffer[MAX_BUFF_SIZE];

	WSABUF m_recv_wsabuf;
	char   m_recv_buffer[MAX_BUFF_SIZE];

	char m_packet_buffer[MAX_PACKET_SIZE];

	unsigned long m_packet_size;
	unsigned long m_saved_packet_size;

	unsigned long m_io_flag = 0;

public:
	BossLockerLayer2D(BossLockerLayer3D* layer);
	~BossLockerLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnMouseMovedEvent(MouseMovedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);
	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnServerPacketEvent(ServerPacketEvent& event);
	
	void ProcessPacket(char* ptr);

	void InputProcess();
};
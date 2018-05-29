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

class TestGameLayer2D : public Layer2D
{
public:
	Panel* m_panel;

	Progressbar* m_bossHPBar;

	Sprite* m_test;

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
	TestGameLayer2D();
	~TestGameLayer2D();

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
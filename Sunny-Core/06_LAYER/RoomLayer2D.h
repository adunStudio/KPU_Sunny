#pragma once

#include <iostream>
#include <SUNNY.h>
#include "../06_LAYER/TestGameLayer3D.h"
#include "../07_SERVER/BossLockerProtocol.h"
#include "../07_SERVER/BossLocker.h"
#include "../05_GAME/Player.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;
using namespace game;

class RoomLayer2D : public Layer2D
{
public:	
	Panel* m_panel;

	unordered_map<string, Sprite*> m_sprites;
	unordered_map<string, Button*> m_buttons;

	int m_mapSelect = 0;
	
private:
	bool firstTime;

	WSABUF m_send_wsabuf;
	char   m_send_buffer[MAX_BUFF_SIZE];

	WSABUF m_recv_wsabuf;
	char   m_recv_buffer[MAX_BUFF_SIZE];

	char m_packet_buffer[MAX_PACKET_SIZE];

	unsigned long m_packet_size;
	unsigned long m_saved_packet_size;

	unsigned long m_io_flag = 0;

public:
	RoomLayer2D();
	~RoomLayer2D();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnServerPacketEvent(ServerPacketEvent& event);

	void ProcessPacket(char* ptr);

	void ButtonClick();

	void SetPlayer();

	void SelectMap(int index);
	void SelectCharacter(int index);
};
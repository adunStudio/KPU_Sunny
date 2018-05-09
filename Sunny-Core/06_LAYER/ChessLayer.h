#pragma once

#include <iostream>
#include <SUNNY.h>
#include "../07_SERVER/ChessProtocol.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;

#define KNIGHT 1
#define TILE_WIDTH  50
#define TILE_HEIGHT 50

class Chess : public Sprite
{
public:
	int col, row;

	Chess(float x, float y, float width, float height, directx::Texture2D* texture)
		: Sprite(x, y, width, height, texture)
	{

	}
};


class ChessLayer : public Layer2D
{
private:
	vector<Chess*> m_knights;

	int m_id;
	int m_x = 15;
	int m_y = 15;
	int m_windowWidth, m_windowHeight;
	
	Sprite* m_knight;
	
	Label* m_label;

	vec2 m_tilePosition;

	int m_board[8][8];

	Panel* m_panel;

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
	ChessLayer();
	~ChessLayer();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnServerPacketEvent(ServerPacketEvent& event);

	void ProcessPacket(char* ptr);

	void ButtonClick();
};
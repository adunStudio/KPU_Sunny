#include "ChessLayer.h"





ChessLayer::ChessLayer()
	: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f)),
	m_saved_packet_size(0), m_packet_size(0), m_io_flag(0), firstTime(true)
{
	Texture2D* texture = new directx::Texture2D("/TEXTURE/knight.png");

	for (int i = 0; i < MAX_USER; ++i)
	{
		m_knights.push_back(new Chess(250, 300, TILE_WIDTH, TILE_HEIGHT, texture));

		m_knights[i]->SetVisible(false);
		Add(m_knights[i]);

	}


	m_label = new Label("position", 0, 25, RGBA(0, 0, 0, 1));
	Add(m_label);
}

ChessLayer::~ChessLayer()
{

}

void ChessLayer::OnInit(Renderer2D& renderer)
{
	m_windowWidth = Application::GetApplication().GetWindowWidth();
	m_windowHeight = Application::GetApplication().GetWindowHeight();

	m_send_wsabuf.buf = m_send_buffer;
	m_send_wsabuf.len = MAX_BUFF_SIZE;
	m_recv_wsabuf.buf = m_recv_buffer;
	m_recv_wsabuf.len = MAX_BUFF_SIZE;
}

void ChessLayer::OnTick()
{
}

void ChessLayer::OnUpdate(const utils::Timestep& ts)
{
	m_label->SetText("position(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ")");

	m_tilePosition.x = m_x - 5;
	m_tilePosition.y = m_y - 5;

	for (int i = 0; i < MAX_USER; ++i)
	{
		if (i == m_id) continue;

		if (m_knights[i]->IsVisible())
		{
			m_knights[i]->SetPosition(vec2(250 - (m_x - m_knights[i]->col) * TILE_WIDTH, 300 + (m_y - m_knights[i]->row) * TILE_WIDTH));
		}
	}
}

void ChessLayer::OnRender(Renderer2D& renderer)
{
	renderer.FillRect(0, 50, TILE_WIDTH * 11, m_windowHeight, vec4(0.77, 1.0, 1.0, 1));

	for (int i = 0; i < 11; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			int tile_x = i + m_tilePosition.x;
			int tile_y = j + m_tilePosition.y;

			if ((tile_x <0) || (tile_y<0)) continue;

			if (((tile_x >> 2) % 2) == ((tile_y >> 2) % 2))
			{
				renderer.FillRect(TILE_WIDTH * i, m_windowHeight - (TILE_HEIGHT * j) - TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, vec4(1.0f, 1.0f, 1.0f, 1.0f));
				renderer.DrawRect(TILE_WIDTH * i, m_windowHeight - (TILE_HEIGHT * j) - TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, vec4(0.2f, 0.2f, 0.2f, 1.0f));
			}
			else
			{
				renderer.FillRect(TILE_WIDTH * i, m_windowHeight - (TILE_HEIGHT * j) - TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				renderer.DrawRect(TILE_WIDTH * i, m_windowHeight - (TILE_HEIGHT * j) - TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, vec4(0.2f, 0.2f, 0.2f, 1.0f));
			}
		}
	}

}

void ChessLayer::OnEvent(Event& event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&ChessLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<ServerPacketEvent>(METHOD(&ChessLayer::OnServerPacketEvent));
}

bool ChessLayer::OnKeyPressedEvent(KeyPressedEvent& event)
{
	//if (event.GetRepeat()) return false;

	SOCKET socket = Server::GetSocket();

	cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(m_send_buffer);
	packet->type = CS_MOVE;
	packet->size = sizeof(packet);
	m_send_wsabuf.len = sizeof(packet);

	DWORD ioByte;

	packet->dir = 100;

	switch (event.GetKeyCode())
	{
	case SUNNY_KEY_LEFT:
		packet->dir = 2;
		break;
	case SUNNY_KEY_RIGHT:
		packet->dir = 3;
		break;
	case SUNNY_KEY_UP:
		packet->dir = 0;
		break;
	case SUNNY_KEY_DOWN:
		packet->dir = 1;
		break;
	}

	if (packet->dir != 100)
	{
		int ret = WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
		if (ret) {
			int error_code = WSAGetLastError();
			printf("Error while sending packet [%d]", error_code);
		}
	}

	return true;
}



bool ChessLayer::OnServerPacketEvent(ServerPacketEvent& event)
{
	unsigned long ioByte = 0;

	int ret = WSARecv(event.socket, &m_recv_wsabuf, 1, &ioByte, &m_io_flag, NULL, NULL);
	if (ret)
	{
		// Debug System
		int err_code = WSAGetLastError();
		printf("Recv Error [%d]\n", err_code);
	}

	unsigned char* ptr = reinterpret_cast<unsigned char*>(m_recv_buffer);



	while (ioByte != 0)
	{
		if (m_packet_size == 0) m_packet_size = ptr[0];

		if (ioByte + m_saved_packet_size >= m_packet_size)
		{
			memcpy(m_packet_buffer + m_saved_packet_size, ptr, m_packet_size - m_saved_packet_size);
			ProcessPacket(m_packet_buffer);
			ptr += m_packet_size - m_saved_packet_size;
			ioByte -= m_packet_size - m_saved_packet_size;
			m_packet_size = 0;
			m_saved_packet_size = 0;
		}
		else
		{
			memcpy(m_packet_buffer + m_saved_packet_size, ptr, ioByte);
			m_saved_packet_size += ioByte;
			ioByte = 0;
		}
	}

	return true;
}

void ChessLayer::ProcessPacket(char* ptr)
{
	switch (ptr[1])
	{


	case SC_PUT_PLAYER:
	{
		sc_packet_put_player* packet = reinterpret_cast<sc_packet_put_player*>(ptr);

		int id = packet->id;
		int  x = packet->x;
		int  y = packet->y;

		if (firstTime)
		{
			m_id = id; firstTime = false;
			m_knights[m_id]->SetVisible(true);
		}

		std::cout << "SC_PUT_PLAYER: [" << id << "]" << " (" << x << ", " << y << ")" << std::endl;

		if (id == m_id) { m_x = x; m_y = y; }
		else
		{
			m_knights[id]->col = x; 
			m_knights[id]->row = y;
		}

		m_knights[id]->SetVisible(true);


		


		break;
	}

	case SC_POS:
	{
		sc_packet_pos* packet = reinterpret_cast<sc_packet_pos*>(ptr);

		int id = packet->id;
		int  x = packet->x;
		int  y = packet->y;

		std::cout << "SC_POS_PLAYER: [" << id << "]" << " (" << x << ", " << y << ")" << std::endl;

		if (id == m_id) { m_x = x; m_y = y; }
		else
		{
			m_knights[id]->col = x;
			m_knights[id]->row = y;
		}

		break;
	}

	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player* packet = reinterpret_cast<sc_packet_remove_player*>(ptr);

		int id = packet->id;

		std::cout << "SC_REMOVE_PLAYER: [" << id << "]" << std::endl;
		
		if(id != m_id)
			m_knights[id]->SetVisible(false);
	}

	}
}



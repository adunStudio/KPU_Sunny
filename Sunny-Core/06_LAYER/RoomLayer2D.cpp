#include "RoomLayer2D.h"

RoomLayer2D::RoomLayer2D()
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
, m_saved_packet_size(0), m_packet_size(0), m_io_flag(0), firstTime(true)
{
	m_send_wsabuf.buf = m_send_buffer;
	m_send_wsabuf.len = MAX_BUFF_SIZE;
	m_recv_wsabuf.buf = m_recv_buffer;
	m_recv_wsabuf.len = MAX_BUFF_SIZE;

}

RoomLayer2D::~RoomLayer2D()
{

}

void RoomLayer2D::OnInit(Renderer2D & renderer)
{
	m_windowWidth = Application::GetApplication().GetWindowWidth();
	m_windowHeight = Application::GetApplication().GetWindowHeight();

	m_mouse = new Sprite(new Texture2D("/TEXTURE/cursor1.png"));


	m_fps = new Label("fps", m_windowWidth - 5, m_windowHeight - 30, RGBA(1, 1, 1, 0.7), Label::Alignment::RIGHT);

	m_sprites["background"] = new Sprite(0, 0, 1600, 900, TextureManager::Get2D("room_background"));
	m_sprites["layout"] = new Sprite(320, 90, 960, 720, TextureManager::Get2D("room_layout"));

	m_sprites["arrow_left"] = new Sprite(850, 245, 50, 50, TextureManager::Get2D("arrow_left"));
	m_sprites["arrow_left_pressed"] = new Sprite(850, 245, 50, 50, TextureManager::Get2D("arrow_left_pressed"));
	m_sprites["arrow_right"] = new Sprite(1190, 245, 50, 50, TextureManager::Get2D("arrow_right"));
	m_sprites["arrow_right_pressed"] = new Sprite(1190, 245, 50, 50, TextureManager::Get2D("arrow_right_pressed"));

	m_sprites["select_face_14"] = new Sprite(350, 640, 150, 150, TextureManager::Get2D("face_14"));
	m_sprites["select_face_14_pressed"] = new Sprite(350, 640, 150, 150, TextureManager::Get2D("face_14_pressed"));
	m_sprites["select_face_15"] = new Sprite(350, 470, 150, 150, TextureManager::Get2D("face_15"));
	m_sprites["select_face_15_pressed"] = new Sprite(350, 470, 150, 150, TextureManager::Get2D("face_15_pressed"));
	m_sprites["select_face_20"] = new Sprite(350, 300, 150, 150, TextureManager::Get2D("face_20"));
	m_sprites["select_face_20_pressed"] = new Sprite(350, 300, 150, 150, TextureManager::Get2D("face_20_pressed"));

	m_sprites["selected_map"] = new Sprite(840, 160, 410, 230, TextureManager::Get2D("map_plain"));

	m_sprites["player_0"] = new Sprite(520, 490, 300, 300, TextureManager::Get2D("face_14"));
	m_sprites["player_1"] = new Sprite(840, 590, 200, 200, nullptr);
	m_sprites["player_2"] = new Sprite(1050, 590, 200, 200, nullptr);

	m_sprites["player_0_state"] = new Sprite(595, 500, 150, 50, TextureManager::Get2D("master"));
	m_sprites["player_1_state"] = new Sprite(865, 600, 150, 50, TextureManager::Get2D("master"));
	m_sprites["player_2_state"] = new Sprite(1075, 600, 150, 50, TextureManager::Get2D("master"));

	m_sprites["chat"] = new Sprite(350, 110, 900, 30, TextureManager::Get2D("room_chat"));
	m_sprites["start_ready"] = new Sprite(350, 160, 470, 120, TextureManager::Get2D("ready"));
	m_sprites["start_ready_pressed"] = new Sprite(350, 160, 470, 120, TextureManager::Get2D("ready_pressed"));

	m_buttons["button_face_14"] = new Button(m_sprites["select_face_14"], m_sprites["select_face_14_pressed"], [&]() {
		SelectCharacter(CHARACTER_14);
	});
	m_buttons["button_face_15"] = new Button(m_sprites["select_face_15"], m_sprites["select_face_15_pressed"], [&]() {
		SelectCharacter(CHARACTER_15);
	});
	m_buttons["button_face_20"] = new Button(m_sprites["select_face_20"], m_sprites["select_face_20_pressed"], [&]() {
		SelectCharacter(CHARACTER_20);
	});

	m_buttons["button_arrow_left"] = new Button(m_sprites["arrow_left"], m_sprites["arrow_left_pressed"], [&]() {
		m_mapSelect = (m_mapSelect - 1) % 3 < 0 ? 2 : (m_mapSelect - 1) % 3;

		SelectMap(m_mapSelect);
	});
	m_buttons["button_arrow_right"] = new Button(m_sprites["arrow_right"], m_sprites["arrow_right_pressed"], [&]() {
		m_mapSelect = (m_mapSelect + 1) % 3;
		SelectMap(m_mapSelect);
	});

	m_buttons["button_start_ready"] = new Button(m_sprites["start_ready"], m_sprites["start_ready_pressed"], LAMBDA(RoomLayer2D::ButtonClick));


	Add(m_sprites["background"]);
	Add(m_sprites["layout"]);
	//Add(m_fps);

	Add(m_sprites["player_0"]);
	Add(m_sprites["player_1"]);
	Add(m_sprites["player_2"]);

	Add(m_sprites["player_0_state"]);
	Add(m_sprites["player_1_state"]);
	Add(m_sprites["player_2_state"]);

	m_sprites["player_0_state"]->SetVisible(false);
	m_sprites["player_1_state"]->SetVisible(false);
	m_sprites["player_2_state"]->SetVisible(false);

	Add(m_sprites["selected_map"]);

	Add(m_sprites["chat"]);

	m_panel = new Panel();
	{
		m_panel->SetMouse(new Button(m_mouse));
		m_panel->Add(m_buttons["button_face_14"]);
		m_panel->Add(m_buttons["button_face_15"]);
		m_panel->Add(m_buttons["button_face_20"]);
	}

}

void RoomLayer2D::OnTick()
{
	m_fps->SetText(std::to_string(Application::GetApplication().GetFPS()) + "fps");
}

void RoomLayer2D::OnUpdate(const utils::Timestep & ts)
{

}

void RoomLayer2D::OnRender(Renderer2D & renderer)
{
}

void RoomLayer2D::OnEvent(Event & event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&RoomLayer2D::OnMouseMovedEvent));
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&RoomLayer2D::OnKeyPressedEvent));
	dispatcher.Dispatch<ServerPacketEvent>(METHOD(&RoomLayer2D::OnServerPacketEvent));
}


bool RoomLayer2D::OnMouseMovedEvent(MouseMovedEvent& event)
{
	float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
	float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

	maths::vec2 mouse(event.GetX() * scaleX, (Window::GetWindowClass()->GetHeight() - event.GetY()) * scaleY);

	m_mouse->SetPosition(vec2(mouse.x, mouse.y - (32 * scaleY)));

	return false;
}
bool RoomLayer2D::OnKeyPressedEvent(KeyPressedEvent & event)
{
	return false;
}

bool RoomLayer2D::OnServerPacketEvent(ServerPacketEvent& event)
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

void RoomLayer2D::ProcessPacket(char* ptr)
{
	switch (ptr[1])
	{
	case SC_PLAYER_PUT:
	{
		sc_packet_player_put* packet = reinterpret_cast<sc_packet_player_put*>(ptr);

		int id                      = packet->id;
		bool isMaster               = packet->isMaster;
		bool isReady                = packet->isReady;
		short player_type           = packet->player_type;
		int map                     = packet->map;

		m_mapSelect = packet->map;

		switch (m_mapSelect)
		{
		case 0: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_plain")); break;
		case 1: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_ice")); break;
		case 2: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_desert")); break;
		}

		Player* player      = new Player(id);
		player->isConnected = true;
		player->isMaster    = isMaster;
		player->isReady     = isReady;
		player->player_type = player_type;

		if (firstTime)
		{
			BossLocker::player = player;

			if (BossLocker::player->isMaster == true)
			{
				m_panel->Add(m_buttons["button_arrow_left"]);
				m_panel->Add(m_buttons["button_arrow_right"]);

				m_sprites["start_ready"]->SetTexture(TextureManager::Get2D("start"));
				m_sprites["start_ready_pressed"]->SetTexture(TextureManager::Get2D("start_pressed"));
			}

			m_panel->Add(m_buttons["button_start_ready"]);
			firstTime = false;
		}

		BossLocker::players[id] = player;
		
		SetPlayer();

		std::cout << "SC_PLAYER_PUT: [" << id << "]" << std::endl;
		break;
	}

	case SC_PLAYER_TYPE:
	{
		sc_packet_player_type* packet = reinterpret_cast<sc_packet_player_type*>(ptr);

		int id                      = packet->id;
		short player_type           = packet->player_type;
		
		BossLocker::players[id]->player_type = player_type;

		SetPlayer();

		std::cout << "SC_PLAYER_TYPE: [" << id << "] : " << player_type << std::endl;

		break;
	}
	case SC_PLAYER_MAP:
	{
		sc_packet_player_map* packet = reinterpret_cast<sc_packet_player_map*>(ptr);

		m_mapSelect = packet->map;

		switch (m_mapSelect)
		{
		case 0: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_plain")); break;
		case 1: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_ice")); break;
		case 2: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_desert")); break;
		}

		std::cout << "SC_PLAYER_MAP: " << m_mapSelect << std::endl;

		break;
	}

	case SC_PLAYER_READY:
	{		
		sc_packet_player_ready* packet = reinterpret_cast<sc_packet_player_ready*>(ptr);
		int id = packet->id;
		bool isReady = packet->isReady;


		BossLocker::players[id]->isReady = isReady;

		std::cout << "SC_PLAYER_READY: [" << id << "] : " << isReady << std::endl;

		SetPlayer();

		break;
	}

	case SC_PLAYER_REMOVE:
	{
		sc_packet_player_remove* packet = reinterpret_cast<sc_packet_player_remove*>(ptr);
		int id = packet->id;

		delete BossLocker::players[id];
		BossLocker::players[id] = nullptr;

		SetPlayer();

		break;
	}

	case SC_PLAYER_START:
		std::cout << "START!!!" << std::endl;
		Application::GetApplication().PopOverlay(m_panel);
		Application::GetApplication().PopOverlay(this);
		Application::GetApplication().PushOverlay(new TestGameLayer3D());
		break;
	}
}





void RoomLayer2D::SetPlayer()
{
	int a = 0;
	Sprite* sprite;
	bool isReady;

	m_sprites["player_1"]->SetTexture(TextureManager::Get2D("face"));
	m_sprites["player_2"]->SetTexture(TextureManager::Get2D("face"));
	m_sprites["player_1_state"]->SetVisible(false);
	m_sprites["player_2_state"]->SetVisible(false);

	for (int i = 0; i < MAX_USER; ++i)
	{
		if (!BossLocker::players[i])
		{
			continue;
		}

		isReady = BossLocker::players[i]->isReady;

		if (BossLocker::players[i] == BossLocker::player)
		{
			sprite = m_sprites["player_0"];

			if (BossLocker::player->isMaster)
			{
				m_sprites["player_0_state"]->SetTexture(TextureManager::Get2D("master"));
				m_sprites["player_0_state"]->SetVisible(true);
			}
			else
			{
				if (isReady)
				{
					m_sprites["player_0_state"]->SetTexture(TextureManager::Get2D("ready"));
					m_sprites["player_0_state"]->SetVisible(true);
				}
				else
					m_sprites["player_0_state"]->SetVisible(false);
			}

		}
		else
		{
			a++;

			if (a == 1)
			{
				sprite = m_sprites["player_1"];
				if (BossLocker::players[i]->isMaster)
				{
					m_sprites["player_1_state"]->SetTexture(TextureManager::Get2D("master"));
					m_sprites["player_1_state"]->SetVisible(true);
				}
				else
				{
					if (isReady)
					{
						m_sprites["player_1_state"]->SetTexture(TextureManager::Get2D("ready"));
						m_sprites["player_1_state"]->SetVisible(true);
					}
					else
						m_sprites["player_1_state"]->SetVisible(false);

				}
			}
			else
			{
				sprite = m_sprites["player_2"];

				if (BossLocker::players[i]->isMaster)
				{
					m_sprites["player_2_state"]->SetTexture(TextureManager::Get2D("master"));
					m_sprites["player_2_state"]->SetVisible(true);
				}
				else
				{
					if (isReady)
					{
						m_sprites["player_2_state"]->SetTexture(TextureManager::Get2D("ready"));
						m_sprites["player_2_state"]->SetVisible(true);
					}
					else
						m_sprites["player_2_state"]->SetVisible(false);

				}
			}
		}

		switch (BossLocker::players[i]->player_type)
		{
		case CHARACTER_14: sprite->SetTexture(TextureManager::Get2D("face_14")); break;
		case CHARACTER_15: sprite->SetTexture(TextureManager::Get2D("face_15")); break;
		case CHARACTER_20: sprite->SetTexture(TextureManager::Get2D("face_20")); break;
		}
	
	}

}

void RoomLayer2D::ButtonClick()
{
	SOCKET socket = Server::GetSocket();

	if (BossLocker::player->isMaster)
	{
		cs_packet_player_start* packet = reinterpret_cast<cs_packet_player_start*>(m_send_buffer);
		packet->type = CS_PLAYER_START;
		packet->size = sizeof(packet);
		m_send_wsabuf.len = sizeof(packet);

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
	}
	else
	{
		cs_packet_player_ready* packet = reinterpret_cast<cs_packet_player_ready*>(m_send_buffer);
		packet->type = CS_PLAYER_READY;
		packet->size = sizeof(packet);
		m_send_wsabuf.len = sizeof(packet);

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
	}

}

void RoomLayer2D::SelectMap(int index)
{
	SOCKET socket = Server::GetSocket();


	cs_packet_player_map* packet = reinterpret_cast<cs_packet_player_map*>(m_send_buffer);
	packet->type = CS_PLAYER_MAP;
	packet->size = sizeof(packet);
	packet->map = m_mapSelect;
	m_send_wsabuf.len = sizeof(packet);

	DWORD ioByte;

	WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
}

void RoomLayer2D::SelectCharacter(int index)
{
	SOCKET socket = Server::GetSocket();

	cs_packet_player_type* packet = reinterpret_cast<cs_packet_player_type*>(m_send_buffer);
	packet->type = CS_PLAYER_TYPE;
	packet->size = sizeof(packet);
	packet->player_type = index;
	m_send_wsabuf.len = sizeof(packet);

	DWORD ioByte;

	WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
}
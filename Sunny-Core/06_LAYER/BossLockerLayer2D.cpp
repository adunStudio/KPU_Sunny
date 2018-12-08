#include "BossLockerLayer2D.h"
#include "BossLockerLayer3D.h"

BossLockerLayer2D::BossLockerLayer2D(BossLockerLayer3D* layer)
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
, m_saved_packet_size(0), m_packet_size(0), m_io_flag(0), m_layer3D(layer)
{
	m_send_wsabuf.buf = m_send_buffer;
	m_send_wsabuf.len = MAX_BUFF_SIZE;
	m_recv_wsabuf.buf = m_recv_buffer;
	m_recv_wsabuf.len = MAX_BUFF_SIZE;
}

BossLockerLayer2D::~BossLockerLayer2D()
{

}

void BossLockerLayer2D::OnInit(Renderer2D& renderer)
{
	m_panel = new Panel();
	m_bossHPBar = new Progressbar(maths::Rectangle(300, 830, 500, 20));
	m_bossHPBar->SetValue(1);
	m_panel->Add(m_bossHPBar);

	for (int i = 0; i < MAX_USER; ++i)
	{
		if (!BossLocker::players[i]) continue;

		CharacterHpBar* hpBar = new CharacterHpBar(maths::Rectangle(38, 20 + i * 40, 100, 15), BossLocker::players[i]);
		hpBar->SetValue(1);
		m_panel->Add(hpBar);

		switch (BossLocker::players[i]->player_type)
		{
		case CHARACTER_14:

			Add(new Sprite(5, 20 + i * 40, 30, 30, TextureManager::Get2D("face_14")));
			break;
		case CHARACTER_15:
			Add(new Sprite(5, 20 + i * 40, 30, 30, TextureManager::Get2D("face_15")));
			break;
		case CHARACTER_20:
			Add(new Sprite(5, 20 + i * 40, 30, 30, TextureManager::Get2D("face_20")));
			break;
		}
	}
}

void BossLockerLayer2D::OnTick()
{

}

void BossLockerLayer2D::OnUpdate(const utils::Timestep& ts)
{
	InputProcess();
}

void BossLockerLayer2D::OnRender(Renderer2D& renderer)
{

}

void BossLockerLayer2D::OnEvent(Event& event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<ServerPacketEvent>(METHOD(&BossLockerLayer2D::OnServerPacketEvent));
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&BossLockerLayer2D::OnMouseMovedEvent));
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&BossLockerLayer2D::OnKeyPressedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&BossLockerLayer2D::OnMouseReleasedEvent));
}

bool BossLockerLayer2D::OnMouseMovedEvent(MouseMovedEvent& event)
{
	return false;
}

void BossLockerLayer2D::ProcessPacket(char* ptr)
{
	if(m_layer3D->stop) return;

	switch (ptr[1])
	{
		case SC_PLAYER_EXIT:
		{
			m_layer3D->stop = true;

			delete Application::GetApplication().PopLayer(this);

			Application::GetApplication().PushLayer2D(new GameOverLayer2D(m_layer3D));

			break;
		}

		case SC_PLAYER_DEGREE:
		{
			sc_packet_player_degree* packet = reinterpret_cast<sc_packet_player_degree*>(ptr);

			int       id = packet->id;
			float degree = packet->degree;


			BossLocker::players[id]->character->GetTransformComponent()->SetRotation(vec3( 0, degree + 90.0f, 0 ));
			break;
		}

		case SC_PLAYER_POS:
		{
			sc_packet_player_pos* packet = reinterpret_cast<sc_packet_player_pos*>(ptr);

			int       id = packet->id;
			float      x = packet->x;
			float      z = packet->z;


			BossLocker::players[id]->character->GetTransformComponent()->SetPosition(vec3(x, 0, z));
			break;
		}

		case SC_PLAYER_ANIMATION:
		{
			sc_packet_player_animation* packet = reinterpret_cast<sc_packet_player_animation*>(ptr);

			int           id = packet->id;
			int    animation = packet->animation;


			if (animation == ANIMATION_DEAD)
			{
				BossLocker::players[id]->alive = false;
				BossLocker::players[id]->character->SetDie(true);
			}

			BossLocker::players[id]->character->SetAnimation(animation);
			break;
		}

		case SC_BULLET_PUT:
		{
			sc_packet_bullet_put* packet = reinterpret_cast<sc_packet_bullet_put*>(ptr);
			int           id = packet->id;
			int         kind = packet->pattern;
			float         x  = packet->x;
			float         z  = packet->z;

			if (kind == BULLET_PLAYER)
			{
				BossLocker::player_bullets[id]->GetTransformComponent()->SetPosition(vec3(x, 100, z));
				BossLocker::player_bullets[id]->SetVisible(true);
			}
			break;
		}

		case SC_BULLET_POS:
		{
			sc_packet_bullet_pos* packet = reinterpret_cast<sc_packet_bullet_pos*>(ptr);
			int           id = packet->id;
			int         kind = packet->kind;
			float         x = packet->x;
			float         z = packet->z;

			if (kind == BULLET_PLAYER)
			{
				BossLocker::player_bullets[id]->GetTransformComponent()->SetPosition(vec3(x, 100, z));
			}
			break;
		}

		case SC_BULLET_REMOVE:
		{
			sc_packet_bullet_remove* packet = reinterpret_cast<sc_packet_bullet_remove*>(ptr);
			int           id = packet->id;
			int         kind = packet->kind;

			if (kind == BULLET_PLAYER)
			{
				BossLocker::player_bullets[id]->SetVisible(false);
			}
			break;
		}

		case SC_BOSS_HP:
		{
			sc_packet_boss_hp* packet = reinterpret_cast<sc_packet_boss_hp*>(ptr);
			float         hp = packet->hp;
			float max = BOSS_HP;
			m_bossHPBar->SetValue(hp / max );
			break;
		}

		case SC_PLAYER_HP:
		{
			sc_packet_player_hp* packet = reinterpret_cast<sc_packet_player_hp*>(ptr);

			BossLocker::players[packet->id]->hp = packet->hp;

			break;
		}

		case SC_SHOOTER_CHANGE:
		{
			sc_packet_shooter_change* packet = reinterpret_cast<sc_packet_shooter_change*>(ptr);

			BossLocker::shooterIndex = packet->shooter;

			m_layer3D->m_shooters[BossLocker::shooterIndex]->SetEnemyTransform(BossLocker::players[packet->target]->character->GetTransformComponent());
		}
	}
}
bool BossLockerLayer2D::OnMouseReleasedEvent(MouseReleasedEvent& event)
{
	SOCKET socket = Server::GetSocket();

	cs_packet_player_mouse_release* packet = reinterpret_cast<cs_packet_player_mouse_release*>(m_send_buffer);
	packet->type = CS_PLAYER_MOUSE_RELEASE;
	packet->size = sizeof(packet);
	m_send_wsabuf.len = sizeof(packet);

	DWORD ioByte;

	WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
	return false;
}

bool BossLockerLayer2D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (BossLocker::player->alive == false) return false;
	switch (event.GetKeyCode())
	{
		case SUNNY_KEY_SPACE:
			if (BossLocker::player->character->GetAnimation() != ANIMATION_ROLL_BASIC)
			{
				SOCKET socket = Server::GetSocket();

				cs_packet_player_roll* packet = reinterpret_cast<cs_packet_player_roll*>(m_send_buffer);
				packet->type = CS_PLAYER_ROLL;
				packet->size = sizeof(packet);
				m_send_wsabuf.len = sizeof(packet);


				DWORD ioByte;

				WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
			}
			break;

	}

	return false;
}

void BossLockerLayer2D::InputProcess()
{
	if (BossLocker::player->alive == false) return;

	SOCKET socket = Server::GetSocket();
	
	cs_packet_player_move* packet = reinterpret_cast<cs_packet_player_move*>(m_send_buffer);
	packet->type = CS_PLAYER_MOVE;
	packet->size = sizeof(packet);
	packet->dir = MOVE_LEFT;
	m_send_wsabuf.len = sizeof(packet);
	
	auto position = BossLocker::player->character->GetTransformComponent()->GetPosition();

	if (Input::IsKeyPressed(SUNNY_KEY_A) && Input::IsKeyPressed(SUNNY_KEY_W))
	{
		if (position.x < -900 || position.z > 1250) return;

		packet->dir = MOVE_LEFT_UP;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}

	if (Input::IsKeyPressed(SUNNY_KEY_A) && Input::IsKeyPressed(SUNNY_KEY_S))
	{
		if (position.x < -900 || position.z < -570) return;

		packet->dir = MOVE_LEFT_DOWN;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}

	if (Input::IsKeyPressed(SUNNY_KEY_D) && Input::IsKeyPressed(SUNNY_KEY_W))
	{
		if (position.x > 1305 || position.z > 1250) return;

		packet->dir = MOVE_RIGHT_UP;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}

	if (Input::IsKeyPressed(SUNNY_KEY_D) && Input::IsKeyPressed(SUNNY_KEY_S))
	{
		if (position.x > 1305 || position.z < -570) return;

		packet->dir = MOVE_RIGHT_DOWN;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}

	if (Input::IsKeyPressed(SUNNY_KEY_W))
	{
		if (position.z > 1250) return;

		packet->dir = MOVE_UP;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}

	if (Input::IsKeyPressed(SUNNY_KEY_S))
	{
		if (position.z < -570) return;

		packet->dir = MOVE_DOWN;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}

	if (Input::IsKeyPressed(SUNNY_KEY_A))
	{
		if (position.x < -900) return;

		packet->dir = MOVE_LEFT;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}

	if (Input::IsKeyPressed(SUNNY_KEY_D))
	{
		if (position.x > 1305) return;

		packet->dir = MOVE_RIGHT;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}

	if (!Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT) && BossLocker::player->character->GetAnimation() != ANIMATION_IDLE_BASIC && BossLocker::player->character->GetAnimation() !=  ANIMATION_ROLL_BASIC)
	{
		packet->dir = MOVE_STOP;

		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

		return;
	}
}









bool BossLockerLayer2D::OnServerPacketEvent(ServerPacketEvent& event)
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
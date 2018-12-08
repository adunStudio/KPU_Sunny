#include "GameOverLayer2D.h"
#include "RoomLayer2D.h"

GameOverLayer2D::GameOverLayer2D(Layer3D* layer)
	: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{
	layer3d = layer;
}

GameOverLayer2D::~GameOverLayer2D()
{
	
}

void GameOverLayer2D::OnInit(Renderer2D& renderer)
{
	Server::DisConnect();
	MouseLayer2D::SetCursor("cursor");
	
	m_gameOverBackgroundTexture = new Texture2D("/TEXTURE/gameoverbackground.png", DIMENSION::D2);
	m_gameOverBackground = new Sprite(0, 0, 1600, 900, m_gameOverBackgroundTexture);
	Add(m_gameOverBackground);

	m_gameOverTexture = new Texture2D("/TEXTURE/gameover.png", DIMENSION::D2);
	m_gameOver = new Sprite(300, 0, m_gameOverTexture);

	m_state = new Label("Press any key to restart", m_windowWidth / 2, 107, "power", RGBA(1, 1, 1, 0.9), Label::Alignment::CENTER);


	m_index = 0;
	m_count = 0;

	m_characterTextures.push_back(new Texture2D("/TEXTURE/Rooms/face_14_pressed.png", DIMENSION::D2));
	m_characterTextures.push_back(new Texture2D("/TEXTURE/Rooms/face_15_pressed.png", DIMENSION::D2));
	m_characterTextures.push_back(new Texture2D("/TEXTURE/Rooms/face_20_pressed.png", DIMENSION::D2));;
}

void GameOverLayer2D::OnTick()
{
	
}

void GameOverLayer2D::OnUpdate(const utils::Timestep& ts)
{
	if(m_count < 144)
	{
		auto a = new Sprite(snails[m_count].y * 100, snails[m_count].x * 100, 100, 100, m_characterTextures[1]);
		Add(a);

		m_count++;

		if (m_count >= 144)
		{
			delete Application::GetApplication().PopLayer(layer3d);

			Add(m_gameOver);
			Add(m_state);
		}
	}

	m_index++;
	if (m_count >= 144 && m_index >= 40)
	{
		if (m_state->GetVisible())
			m_state->SetVisible(false);
		else
			m_state->SetVisible(true);

		m_index = 0;
	}

}

void GameOverLayer2D::OnRender(Renderer2D& renderer)
{

}

void GameOverLayer2D::OnEvent(Event& event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&GameOverLayer2D::OnMousePressedEvent));
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&GameOverLayer2D::OnKeyPressedEvent));
}


bool GameOverLayer2D::OnMousePressedEvent(MousePressedEvent& event)
{
	if (m_count < 144) return false;

	BossLocker::players[0] = nullptr;
	BossLocker::players[1] = nullptr;
	BossLocker::players[2] = nullptr;
	BossLocker::player = nullptr;
	BossLocker::id = 0;
	BossLocker::player_bullets.clear();
	BossLocker::shooterIndex = 0;
	delete BossLocker::bulletList;
	BossLocker::bulletList = new PoolList(110, 500000);


	Application::GetApplication().PushLayer2D(new RoomLayer2D());

	delete Application::GetApplication().PopLayer(this);

	return false;
}
bool GameOverLayer2D::OnKeyPressedEvent(KeyPressedEvent&   event)
{
	if (m_count < 144) return false;

	BossLocker::players[0] = nullptr;
	BossLocker::players[1] = nullptr;
	BossLocker::players[2] = nullptr;
	BossLocker::player = nullptr;
	BossLocker::id = 0;
	BossLocker::player_bullets.clear();
	BossLocker::shooterIndex = 0;
	delete BossLocker::bulletList;
	BossLocker::bulletList = new PoolList(110, 500000);

	Application::GetApplication().PushLayer2D(new RoomLayer2D());

	delete Application::GetApplication().PopLayer(this);

	return false;
}
#include "ChessLayer.h"

ChessLayer::ChessLayer()
	: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{

	for (int y = 0; y < 8; ++y)
		memset(m_board[y], 0, sizeof(int) * 8);

	m_board[3][3] = KNIGHT;

	m_knight = new Sprite(0, 0, 45, 45, directx::Texture2D::CreateFromFile("/textures/knight.png"));
	Add(new Sprite(360, 360, 360, 360, directx::Texture2D::CreateFromFile("/textures/board.png")));
	Add(m_knight);
}

ChessLayer::~ChessLayer()
{
	
}

void ChessLayer::OnInit(Renderer2D& renderer)
{
	int windowWidth = Application::GetApplication().GetWindowWidth();
	int windowHeight = Application::GetApplication().GetWindowHeight();
}

void ChessLayer::OnTick()
{
}

void ChessLayer::OnUpdate(const utils::Timestep& ts)
{
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (m_board[y][x] == KNIGHT)
			{
				m_knight->SetPosition({ (float)(x) * 90 + 45, (float)(7 - y) * 90 + 45 });
			}
		}
	}
}

void ChessLayer::OnRender(Renderer2D& renderer)
{

}
		

void ChessLayer::OnEvent(Event& event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&ChessLayer::OnKeyPressedEvent));
}

bool ChessLayer::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (event.GetRepeat())
		return false;

	int knight_x = 0; int knight_y = 0;

	for (int y = 0; y < 8; ++y)
		for (int x = 0; x < 8; ++x)
			if (m_board[y][x] == KNIGHT)
			{
				knight_x = x;
				knight_y = y;
			}

	switch (event.GetKeyCode())
	{
	case SUNNY_KEY_LEFT:
		knight_x = max(knight_x -1, 0);
		break;
	case SUNNY_KEY_RIGHT:
		knight_x = min(knight_x + 1, 7);
		break;
	case SUNNY_KEY_UP:
		knight_y = max(knight_y - 1, 0);
		break;
	case SUNNY_KEY_DOWN:
		knight_y = min(knight_y + 1, 7);
		break;
	}

	for (int y = 0; y < 8; ++y)
		memset(m_board[y], 0, sizeof(int) * 8);

	m_board[knight_y][knight_x] = KNIGHT;

	return true;
}



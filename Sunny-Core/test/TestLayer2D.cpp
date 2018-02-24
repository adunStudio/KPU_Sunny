#include "TestLayer2D.h"

TestLayer2D::TestLayer2D()
: Layer2D(maths::mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f))
{
	cout << "TestLayer::TestLayer()" << endl;
}

TestLayer2D::~TestLayer2D()
{

}

void TestLayer2D::OnInit(Renderer2D& renderer)
{
	cout << "TestLayer::OnInit()" << endl;
	Add(new Sprite(0.0f, 0.0f, 0.9f,0.9f, RGBA(1, 0, 0, 1)));

	m_timeTable = new Sprite(-5, -5, 3, 3, directx::Texture2D::CreateFromFIle("/textures/time.png"));
	Add(m_timeTable);
}

void TestLayer2D::OnTick()
{
	cout << "TestLayer::OnTick()" << endl;
	std::cout << "TPS: " << Application::GetApplication().GetUPS() << std::endl;
	std::cout << "FPS: " << Application::GetApplication().GetFPS() << std::endl;
}

void TestLayer2D::OnUpdate(const utils::Timestep& ts)
{
	//cout << "TestLayer::OnUpdate()" << endl;
}

void TestLayer2D::OnRender(Renderer2D& renderer)
{
	//cout << "TestLayer::OnRender()" << endl;
	renderer.DrawLine(0, 0, 3, 3, maths::vec4(0, 0, 1, 0.3), 0.1);
	renderer.DrawRect(0, 0, 3, 3, maths::vec4(0, 0, 1, 0.3));
	renderer.FillRect(0, 0, 3, 3, maths::vec4(0, 0, 1, 0.3));
}

void TestLayer2D::OnEvent(Event& event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&TestLayer2D::OnKeyPressedEvent));
}

bool TestLayer2D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (event.GetRepeat())
		return false;

	switch (event.GetKeyCode())
	{
	case SUNNY_KEY_LEFT:
		m_timeTable->SetPosition(maths::vec2(m_timeTable->GetPosition().x - 1, m_timeTable->GetPosition().y));
		break;
	case SUNNY_KEY_RIGHT:
		m_timeTable->SetPosition(maths::vec2(m_timeTable->GetPosition().x + 1, m_timeTable->GetPosition().y));
		break;
	case SUNNY_KEY_UP:
		m_timeTable->SetPosition(maths::vec2(m_timeTable->GetPosition().x , m_timeTable->GetPosition().y + 1));
		break;
	case SUNNY_KEY_DOWN:
		m_timeTable->SetPosition(maths::vec2(m_timeTable->GetPosition().x, m_timeTable->GetPosition().y - 1));
		break;
	}


	return false;
}
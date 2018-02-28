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

	m_layerIndex = 0;

	m_layers.push_back(new NPCLayer());
	m_layers.push_back(new BossLayer());

	m_layers[1]->SetVisible(false);
	m_layers[1]->SetActive(false);

	Application::GetApplication().PushLayer(m_layers[0]);
	Application::GetApplication().PushLayer(m_layers[1]);
	
	m_panel = new Panel();
	
	m_logo = new Sprite(-5, -5, 5, 2.5, directx::Texture2D::CreateFromFIle("/textures/logo.png"));
	m_fps = new Label("fps", 15.7, 8.5, RGBA(1, 1, 1, 0.8));
	m_fps->SetAlignment(Label::Alignment::RIGHT);

	
	Button* button = new Button("Test Button", maths::Rectangle(-14.83, -7.8, 1, 1));

	button->SetAction([&]() {
		ButtonEvent1();
	});

	Add(m_logo);

	Add(m_fps);
	m_panel->Add(button);

}

void TestLayer2D::OnTick()
{
	m_fps->SetText(std::to_string(Application::GetApplication().GetFPS()) + "fps");
	//cout << "TestLayer::OnTick()" << endl;
	//std::cout << "TPS: " << Application::GetApplication().GetTPS() << std::endl;
	//std::cout << "FPS: " << Application::GetApplication().GetFPS() << std::endl;
}

void TestLayer2D::OnUpdate(const utils::Timestep& ts)
{
	//cout << "TestLayer::OnUpdate()" << endl;
}

void TestLayer2D::OnRender(Renderer2D& renderer)
{
	//cout << "TestLayer::OnRender()" << endl;
	//renderer.DrawLine(0, 0, 0, 10, maths::vec4(0, 0, 1, 0.5), 0.2);
	//renderer.DrawLine(0, 0, 10, 0, maths::vec4(1, 0, 0, 0.5), 0.2);
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
		m_logo->SetPosition(maths::vec2(m_logo->GetPosition().x - 1, m_logo->GetPosition().y));
		break;
	case SUNNY_KEY_RIGHT:
		m_logo->SetPosition(maths::vec2(m_logo->GetPosition().x + 1, m_logo->GetPosition().y));
		break;
	case SUNNY_KEY_UP:
		m_logo->SetPosition(maths::vec2(m_logo->GetPosition().x , m_logo->GetPosition().y + 1));
		break;
	case SUNNY_KEY_DOWN:
		m_logo->SetPosition(maths::vec2(m_logo->GetPosition().x, m_logo->GetPosition().y - 1));
		break;
	}


	return false;
}

void TestLayer2D::ButtonEvent1()
{
	m_layers[m_layerIndex]->SetVisible(false);
	m_layers[m_layerIndex]->SetActive(false);
	m_layerIndex = m_layerIndex == 0 ? 1 : 0;
	m_layers[m_layerIndex]->SetVisible(true);
	m_layers[m_layerIndex]->SetActive(true);
}
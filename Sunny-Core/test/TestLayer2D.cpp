#include "TestLayer2D.h"

TestLayer2D::TestLayer2D()
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{

}

TestLayer2D::~TestLayer2D()
{

}

void TestLayer2D::OnInit(Renderer2D& renderer)
{
	int windowWidth =  Application::GetApplication().GetWindowWidth();
	int windowHeight = Application::GetApplication().GetWindowHeight();

	m_layerIndex = 0;

	m_layers.push_back(new NPCLayer());
	m_layers.push_back(new BossLayer());

	m_layers[1]->SetVisible(false);
	m_layers[1]->SetActive(false);

	Application::GetApplication().PushLayer(m_layers[0]);
	Application::GetApplication().PushLayer(m_layers[1]);
	
	m_logo  = new Sprite(230, 368/4 + 520, 1037/4, 368/4, directx::Texture2D::CreateFromFIle("/textures/logo.png"));
	m_fps   = new Label("fps", windowWidth - 5,  windowHeight - 15, RGBA(1, 1, 1, 0.8));
	m_fps->SetAlignment(Label::Alignment::RIGHT);
	
	m_panel = new Panel();
	Slider* slider = new Slider(maths::Rectangle(200, 200, 30, 100), true);
	Button* button = new Button("Test Button", maths::Rectangle(40, 40, 20, 20));

	button->SetAction([&]() {
		//ButtonEvent1();
		m_layers[m_layerIndex]->SetVisible(false);
		m_layers[m_layerIndex]->SetActive(false);
		m_layerIndex = m_layerIndex == 0 ? 1 : 0;
		m_layers[m_layerIndex]->SetVisible(true);
		m_layers[m_layerIndex]->SetActive(true);
	});

	Add(m_logo);
	Add(m_fps);
	m_panel->Add(button);
	m_panel->Add(slider);
}

void TestLayer2D::OnTick()
{
	m_fps->SetText(std::to_string(Application::GetApplication().GetFPS()) + "fps");
}

void TestLayer2D::OnUpdate(const utils::Timestep& ts)
{
}

void TestLayer2D::OnRender(Renderer2D& renderer)
{
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
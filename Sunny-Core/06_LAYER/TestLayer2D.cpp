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

	//m_layers.push_back(new NPCLayer());
	//m_layers.push_back(new BossLayer());
	m_layers.push_back(new TestLayer3D);
	//m_layers[1]->SetVisible(false);
	//m_layers[1]->SetActive(false);

	Application::GetApplication().PushLayer(m_layers[0]);
	//Application::GetApplication().PushLayer(m_layers[1]);

	m_logo  = new Sprite(230, 368/4 + 520, 1037/4, 368/4, directx::Texture2D::CreateFromFile("/TEXTURE/logo.png"));
	m_fps   = new Label("fps", windowWidth - 5,  windowHeight - 15, RGBA(1, 1, 1, 0.8));
	m_fps->SetAlignment(Label::Alignment::RIGHT);
	
	m_panel = new Panel();
	
	Button* button = new Button(" Change Button", maths::Rectangle(65, 25, 60, 20));
	button->SetAction(LAMBDA(this->ButtonEvent1));

	m_slider1 = new Slider(maths::Rectangle(20, 200, 10, 100), true);
	m_slider2 = new Slider(maths::Rectangle(640, 25, 300, 20), false);
	m_progressbar = new Progressbar(maths::Rectangle(640, 75, 300, 20));

	m_progressbar->SetCallback([](float) {
		//std::cout << "Complete" << std::endl;
	});

	Dialog* dialog = new Dialog(maths::Rectangle(1150, 500, 100, 100), "   Basic Dialog", "Test");
	dialog->SetCloseAction([&, dialog]() {
		m_panel->Remove(dialog);
	});

	

	Add(m_logo);
	Add(m_fps);

	m_panel->Add(button);
	m_panel->Add(m_slider1);
	m_panel->Add(m_slider2);
	m_panel->Add(dialog);
	m_panel->Add(m_progressbar);
}

void TestLayer2D::OnTick()
{
	m_fps->SetText(std::to_string(Application::GetApplication().GetFPS()) + "fps");
}

void TestLayer2D::OnUpdate(const utils::Timestep& ts)
{
	/*float length, frame;

	if (dynamic_cast<NPCLayer*>(m_layers[m_layerIndex]))
	{
		length = dynamic_cast<NPCLayer*>(m_layers[m_layerIndex])->m_entity->GetMesh()->GetAnimationLength();
		frame = dynamic_cast<NPCLayer*>(m_layers[m_layerIndex])->m_entity->GetMesh()->GetCurrentFrame();
	}
	else if (dynamic_cast<BossLayer*>(m_layers[m_layerIndex]))
	{
		length = dynamic_cast<BossLayer*>(m_layers[m_layerIndex])->m_entity->GetMesh()->GetAnimationLength();
		frame = dynamic_cast<BossLayer*>(m_layers[m_layerIndex])->m_entity->GetMesh()->GetCurrentFrame();
	}
	*/
	//m_progressbar->SetValue((frame+1) / length);
	//m_slider2->SetValue( (frame+1) / length);
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
	//if (event.GetRepeat())
		//return false;

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
	//m_layers[m_layerIndex]->SetVisible(false);
	//m_layers[m_layerIndex]->SetActive(false);
	//m_layerIndex = m_layerIndex == 0 ? 1 : 0;
	//m_layers[m_layerIndex]->SetVisible(true);
	//m_layers[m_layerIndex]->SetActive(true);
}
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

	// ¸ÊÅø ·¹ÀÌ¾î
	m_testLayer3D = new TestLayer3D(*this);
	Application::GetApplication().PushLayer(m_testLayer3D);

	// FPS
	m_fps   = new Label("fps", windowWidth - 5,  windowHeight - 15, RGBA(0, 0, 0, 0.8));
	m_fps->SetAlignment(Label::Alignment::RIGHT);
	//Add(m_fps);

	// Model
	model_position = new Label("[T] x: 10 \t\t y: 10 \t\t z:10 \t", windowWidth - 10, windowHeight - 75,  FontManager::Get("nanum", 24), RGBA(1, 1, 1, 0.8), Label::Alignment::RIGHT);
	model_rotation = new Label("[R] x: 10 \t\t y: 10 \t\t z:10 \t", windowWidth - 10, windowHeight - 100, FontManager::Get("nanum", 24), RGBA(1, 1, 1, 0.8), Label::Alignment::RIGHT);
	model_scale    = new Label("[S] x: 10 \t\t y: 10 \t\t z:10 \t", windowWidth - 10, windowHeight - 125, FontManager::Get("nanum", 24), RGBA(1, 1, 1, 0.8), Label::Alignment::RIGHT);
	Add(model_position);
	Add(model_rotation);
	Add(model_scale);
	
	m_model_panel = new Panel();


	m_panel = new Panel();
	
	Button* button = new Button("    SAVE", maths::Rectangle(50, 25, 40, 20));
	button->SetAction(LAMBDA(this->SaveEvent));

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


	m_panel->Add(button);
	//m_panel->Add(m_slider1);
	//m_panel->Add(m_slider2);
	//m_panel->Add(dialog);
	//m_panel->Add(m_progressbar);
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
	//if (event.GetRepeat())
		//return false;

	/*switch (event.GetKeyCode())
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
	}*/

	return false;
}

void TestLayer2D::SaveEvent()
{
	//m_layers[m_layerIndex]->SetVisible(false);
	//m_layers[m_layerIndex]->SetActive(false);
	//m_layerIndex = m_layerIndex == 0 ? 1 : 0;
	//m_layers[m_layerIndex]->SetVisible(true);
	//m_layers[m_layerIndex]->SetActive(true);
}
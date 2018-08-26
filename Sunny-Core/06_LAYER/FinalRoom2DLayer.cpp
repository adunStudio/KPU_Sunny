#include "FinalRoom2DLayer.h"

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}
FinalRoom2DLayer::FinalRoom2DLayer()
	: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{
}

FinalRoom2DLayer::~FinalRoom2DLayer()
{
}

void FinalRoom2DLayer::OnInit(Renderer2D & renderer)
{
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

	m_buttons["button_start_ready"] = new Button(m_sprites["start_ready"], m_sprites["start_ready_pressed"], LAMBDA(FinalRoom2DLayer::ButtonClick));


	Add(m_sprites["background"]);
	Add(m_sprites["layout"]);

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
		m_panel->Add(m_buttons["button_face_14"]);
		m_panel->Add(m_buttons["button_face_15"]);
		m_panel->Add(m_buttons["button_face_20"]);
	}

	m_panel->Add(m_buttons["button_arrow_left"]);
	m_panel->Add(m_buttons["button_arrow_right"]);

	m_sprites["start_ready"]->SetTexture(TextureManager::Get2D("start"));
	m_sprites["start_ready_pressed"]->SetTexture(TextureManager::Get2D("start_pressed"));
	m_panel->Add(m_buttons["button_start_ready"]);

	m_sprites["player_1"]->SetTexture(TextureManager::Get2D("face"));
	m_sprites["player_2"]->SetTexture(TextureManager::Get2D("face"));
	m_sprites["player_1_state"]->SetVisible(false);
	m_sprites["player_2_state"]->SetVisible(false);

	m_sprites["player_0_state"]->SetTexture(TextureManager::Get2D("master"));
	m_sprites["player_0_state"]->SetVisible(true);

	//FinalGame::characterType = CHARACTER_14;
}

void FinalRoom2DLayer::OnTick()
{
}

void FinalRoom2DLayer::OnUpdate(const utils::Timestep & ts)
{

}

void FinalRoom2DLayer::OnRender(Renderer2D & renderer)
{
}

void FinalRoom2DLayer::OnEvent(Event & event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&FinalRoom2DLayer::OnKeyPressedEvent));
}

bool FinalRoom2DLayer::OnKeyPressedEvent(KeyPressedEvent & event)
{
	return false;
}


void FinalRoom2DLayer::ButtonClick()
{
	delete m_panel;
	delete Application::GetApplication().PopLayer(this);
	Application::GetApplication().PushLayer3D(new ParticleLayer3D());
}

void FinalRoom2DLayer::SelectMap(int index)
{
	m_mapSelect = index;

	switch (m_mapSelect)
	{
	case 0: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_plain")); break;
	case 1: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_ice")); break;
	case 2: m_sprites["selected_map"]->SetTexture(TextureManager::Get2D("map_desert")); break;
	}
}

void FinalRoom2DLayer::SelectCharacter(int index)
{

	FinalGame::characterType = index;

	Sprite* sprite;

	sprite = m_sprites["player_0"];

	

	switch (index)
	{
	case CHARACTER_14: sprite->SetTexture(TextureManager::Get2D("face_14")); break;
	case CHARACTER_15: sprite->SetTexture(TextureManager::Get2D("face_15")); break;
	case CHARACTER_20: sprite->SetTexture(TextureManager::Get2D("face_20")); break;
	}
}
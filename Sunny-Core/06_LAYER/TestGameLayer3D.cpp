#include "TestGameLayer3D.h"
#include "TestLayer2D.h"
#include "MouseLayer2D.h"

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}

TestGameLayer3D::TestGameLayer3D()
{
	m_send_wsabuf.buf = m_send_buffer;
	m_send_wsabuf.len = MAX_BUFF_SIZE;
}

TestGameLayer3D::~TestGameLayer3D()
{

}

void TestGameLayer3D::OnInit(Renderer3D& renderer)
{
	m_layer2D = new TestGameLayer2D();
	Application::GetApplication().PushLayer2D(m_layer2D);

	MouseLayer2D::SetCursor("attack");

	// ¶óÀÌÆ®
	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.3, 0.3, 0.3), 2, vec4(1.f, 1.f, 1.f, 1.f));
	lights->Add(light);
	PushLightSetup(lights);

	std::string skyBoxFiles[1] =
	{
		"/CUBE/skybox.png",
	};

	TextureCube* environment = new TextureCube("skybox", skyBoxFiles, 1);
	Shader* skybox = Shader::CreateFromFile("skybox", "/HLSL/03_skybox.hlsl");
	Material* skyboxMaterial = new Material(skybox);
	skybox->Bind();
	m_SkyboxMaterial = new MaterialInstance(skyboxMaterial);
	m_SkyboxMaterial->SetTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = new Entity(MeshFactory::CreateQuad(-1, -1, 2, 2));
	skyboxEntity->SetMaterial(m_SkyboxMaterial);
	SetSkybox(skyboxEntity);

	std::string mapData = system::FileSystem::ReadTextFile("/JSON/MAP/map1.json");

	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(mapData.c_str(), root);

	if (parsingSuccessful)
	{
		std::cout << "ÆÄ½ÌÆÄ½Ì" << std::endl;

		for (int i = 0; i < root.size(); ++i)
		{
			std::string name = root[i]["name"].asString();

			name[0] = toupper(name[0]);

			maths::vec3 translation = vec3(root[i]["position"]["x"].asFloat(), root[i]["position"]["y"].asFloat(), root[i]["position"]["z"].asFloat());
			maths::vec3 rotation = vec3(root[i]["rotation"]["x"].asFloat(), root[i]["rotation"]["y"].asFloat(), root[i]["rotation"]["z"].asFloat());
			maths::vec3 scale = vec3(root[i]["scale"]["x"].asFloat(), root[i]["scale"]["y"].asFloat(), root[i]["scale"]["z"].asFloat());

			translation *= 100;
			translation.x += 900;

			mat4 position = mat4::Identity();

			auto a = new Model3D(name);

			//m_mapObjects.push_back(a);

			a->GetTransformComponent()->SetPosition(translation);
			a->GetTransformComponent()->Rotate(rotation);
			a->GetTransformComponent()->SetScale(scale);

			AddStatic(a);
		}
	}


	m_sphere = new Model("/OBJ/sphere.obj");
	Entity* e = new Entity(m_sphere->GetMesh(), RGBA(1.0, 0, 0, 1.0));
	Add(e);

	m_animations[14].push_back(ModelManager::GetMesh("14_idle_basic"));
	m_animations[14].push_back(ModelManager::GetMesh("14_dead"));
	m_animations[14].push_back(ModelManager::GetMesh("14_attack_basic"));
	m_animations[14].push_back(ModelManager::GetMesh("14_roll_basic"));
	m_animations[14].push_back(ModelManager::GetMesh("14_run_attack"));
	m_animations[14].push_back(ModelManager::GetMesh("14_run_basic"));
	m_animations[14].push_back(ModelManager::GetMesh("14_idle_attack"));

	m_animations[15].push_back(ModelManager::GetMesh("15_idle_basic"));
	m_animations[15].push_back(ModelManager::GetMesh("15_dead"));
	m_animations[15].push_back(ModelManager::GetMesh("15_attack_basic"));
	m_animations[15].push_back(ModelManager::GetMesh("15_roll_basic"));
	m_animations[15].push_back(ModelManager::GetMesh("15_run_attack"));
	m_animations[15].push_back(ModelManager::GetMesh("15_run_basic"));
	m_animations[15].push_back(ModelManager::GetMesh("15_idle_attack"));

	m_animations[20].push_back(ModelManager::GetMesh("20_idle_basic"));
	m_animations[20].push_back(ModelManager::GetMesh("20_dead"));
	m_animations[20].push_back(ModelManager::GetMesh("20_attack_basic"));
	m_animations[20].push_back(ModelManager::GetMesh("20_roll_basic"));
	m_animations[20].push_back(ModelManager::GetMesh("20_run_attack"));
	m_animations[20].push_back(ModelManager::GetMesh("20_run_basic"));
	m_animations[20].push_back(ModelManager::GetMesh("20_idle_attack"));




	for (int i = 0; i < MAX_USER; ++i)
	{
		if (!BossLocker::players[i]) continue;

		switch (BossLocker::players[i]->player_type)
		{
		case CHARACTER_14:
			BossLocker::players[i]->character = new Animation3D(m_animations[14], new Texture2D("/TEXTURE/14_body.png"), new Texture2D("/TEXTURE/14_face.png"));
			BossLocker::players[i]->character->GetTransformComponent()->SetScale(vec3(100, 100, 100));
			Add(BossLocker::players[i]->character);
			break;
		case CHARACTER_15:
			BossLocker::players[i]->character = new Animation3D(m_animations[15], new Texture2D("/TEXTURE/15_body.png"), new Texture2D("/TEXTURE/15_face.png"));
			BossLocker::players[i]->character->GetTransformComponent()->SetScale(vec3(100, 100, 100));
			Add(BossLocker::players[i]->character);
			break;
		case CHARACTER_20:
			BossLocker::players[i]->character = new Animation3D(m_animations[20], new Texture2D("/TEXTURE/20_body.png"), new Texture2D("/TEXTURE/20_face.png"));
			BossLocker::players[i]->character->GetTransformComponent()->SetScale(vec3(100, 100, 100));
			Add(BossLocker::players[i]->character);
			break;
		}
	}


	Model* player_bullet = new Model("/OBJ/sphere2.obj");

	for (int i = 0; i < MAX_USER_BULLET; ++i)
	{
		Entity* b = new Entity(player_bullet->GetMesh(), RGBA(1.0f, 0.0f, 0.0f, 1.0f));
		b->GetTransformComponent()->SetScale(vec3(20, 20, 20));
		b->SetVisible(false);
		BossLocker::player_bullets.push_back(b);
		Add(b);
	}
	
	//Model* boss_model = new Model("/SUN/Characters/b20_idle_basic");
	m_boss = new Entity(ModelManager::GetMesh("20_idle_basic"), new Texture2D("/TEXTURE/boss_texture.png"));
	m_boss->GetTransformComponent()->SetPosition(vec3(-300, 0, 0));
	m_boss->GetTransformComponent()->SetRotation(vec3(0, 180, 0));
	m_boss->GetTransformComponent()->SetScale(vec3(100, 100, 100));
	Add(m_boss);

	SetCamera(new QuaterCamera(maths::mat4::Perspective(65.0f, 1600.0f / 900.0f, 0.1f, 1000.0f), BossLocker::player->character));
}

void TestGameLayer3D::OnTick()
{

}

void TestGameLayer3D::OnUpdate(const utils::Timestep& ts)
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (!BossLocker::players[i]) continue;
		BossLocker::players[i]->character->PlayAnimation();
	}

	if (Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT) && BossLocker::player->character->GetAnimation() != ANIMATION_ROLL_BASIC)
	{
		SOCKET socket = Server::GetSocket();

		cs_packet_player_attack* packet = reinterpret_cast<cs_packet_player_attack*>(m_send_buffer);
		packet->type   = CS_PLAYER_ATTACK;
		packet->size   = sizeof(packet);
		packet->degree = m_degree;
		m_send_wsabuf.len = sizeof(packet);
		
		DWORD ioByte;

		WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
	}


	mat4 vp = GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));
}

void TestGameLayer3D::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&TestGameLayer3D::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&TestGameLayer3D::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&TestGameLayer3D::OnMousePressedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&TestGameLayer3D::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&TestGameLayer3D::OnMouseReleasedEvent));
}

bool TestGameLayer3D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	return false;
}

bool TestGameLayer3D::OnKeyReleasedEvent(KeyReleasedEvent& event)
{
	return false;
}

bool TestGameLayer3D::OnMousePressedEvent(MousePressedEvent& event)
{
	float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
	float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

	maths::vec2 mouse(event.GetX() * scaleX, event.GetY() * scaleY);

	return false;
}

bool TestGameLayer3D::OnMouseReleasedEvent(MouseReleasedEvent& event)
{
	return false;
}

bool TestGameLayer3D::OnMouseMovedEvent(MouseMovedEvent& event)
{

	maths::vec2 mouse_xy(event.GetX(), event.GetY());

	vec3 P = BossLocker::player->character->GetTransformComponent()->GetPosition();
	mat4 VP = GetCamera()->GetViewMatrix() * GetCamera()->GetProjectionMatrix();

	float x = P.x * VP.a11 + P.y * VP.a21 + P.z * VP.a31 + VP.a41;
	float y = P.x * VP.a12 + P.y * VP.a22 + P.z * VP.a31 + VP.a42;
	float z = P.x * VP.a13 + P.y * VP.a23 + P.z * VP.a31 + VP.a43;
	float w = P.x * VP.a14 + P.y * VP.a24 + P.z * VP.a31 + VP.a44;

	x /= w;
	y /= w;
	z /= w;

	double screenX = Window::GetWindowClass()->GetWidth()   * (x + 1.0f) / 2.0f + 0;
	double screenY = Window::GetWindowClass()->GetHeight()  * (2.0f - (y + 1.0f)) / 2.0f + 0;



	m_radian = maths::atan2(mouse_xy.y - screenY, mouse_xy.x - screenX);

	m_degree = m_radian * 180 / maths::SUNNY_PI;

	BossLocker::player->character->GetTransformComponent()->SetRotation(vec3(0, m_degree + 90, 0));


	SOCKET socket = Server::GetSocket();

	cs_packet_player_degree* packet = reinterpret_cast<cs_packet_player_degree*>(m_send_buffer);
	packet->type = CS_PLAYER_DEGREE;
	packet->size = sizeof(packet);
	packet->degree = m_degree;
	m_send_wsabuf.len = sizeof(packet);

	DWORD ioByte;

	WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

	return false;
}
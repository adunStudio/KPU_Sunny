#include "BossLockerLayer3D.h"
#include "TestLayer2D.h"
#include "MouseLayer2D.h"

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}

BossLockerLayer3D::BossLockerLayer3D()
{
	m_send_wsabuf.buf = m_send_buffer;
	m_send_wsabuf.len = MAX_BUFF_SIZE;
}

BossLockerLayer3D::~BossLockerLayer3D()
{
	
}

void BossLockerLayer3D::OnInit(Renderer3D& renderer)
{
	m_socket = Server::GetSocket();

	MouseLayer2D::SetCursor("attack");

	start = chrono::high_resolution_clock::now();

	m_bulletParticle = new BulletParticle(new directx::Texture2D("/TEXTURE/particle.png"));
	Add(m_bulletParticle);

	// 2D Layer
	{
		m_layer2D = new BossLockerLayer2D(this);
		Application::GetApplication().PushLayer2D(m_layer2D);
	}
	
	// Light
	{
		LightSetup* lights = new LightSetup();
		Light* light       = new Light(vec3(0.3, 0.3, 0.3), 2, vec4(1.f, 1.f, 1.f, 1.f));
		lights->Add(light);
		PushLightSetup(lights);

	}
	
	// SkyBox
	{
		std::string skyBoxFiles[1] = { "/CUBE/skybox.png", };
		TextureCube* environment = new TextureCube("skybox", skyBoxFiles, 1);
		Shader* skybox = Shader::CreateFromFile("skybox", "/HLSL/03_skybox.hlsl");
		Material* skyboxMaterial = new Material(skybox);
		skybox->Bind();
		m_skyboxMaterial = new MaterialInstance(skyboxMaterial);
		m_skyboxMaterial->SetTexture("u_EnvironmentMap", environment);
		Entity* skyboxEntity = new Entity(MeshFactory::CreateQuad(-1, -1, 2, 2));
		skyboxEntity->SetMaterial(m_skyboxMaterial);
		SetSkybox(skyboxEntity);
	}

	// Map
	{
		std::string mapData = system::FileSystem::ReadTextFile("/JSON/MAP/map4.json");

		Json::Value root;
		Json::Reader reader;

		bool parsingSuccessful = reader.parse(mapData.c_str(), root);

		if (parsingSuccessful)
		{
			for (int i = 0; i < root.size(); ++i)
			{
				std::string name = root[i]["name"].asString();

				name[0] = toupper(name[0]);

				maths::vec3 translation = vec3(root[i]["position"]["x"].asFloat(), root[i]["position"]["y"].asFloat(), root[i]["position"]["z"].asFloat());
				maths::vec3 rotation = vec3(root[i]["rotation"]["x"].asFloat(), root[i]["rotation"]["y"].asFloat(), root[i]["rotation"]["z"].asFloat());
				maths::vec3 scale = vec3(root[i]["scale"]["x"].asFloat(), root[i]["scale"]["y"].asFloat(), root[i]["scale"]["z"].asFloat());

				mat4 position = mat4::Identity();

				auto mapObject = new Model3D(name);

				mapObject->GetTransformComponent()->SetPosition(translation);
				mapObject->GetTransformComponent()->Rotate(rotation);
				mapObject->GetTransformComponent()->SetScale(scale);

				AddStatic(mapObject);
			}
		}
	}

	// Animation
	{
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
	}
	

	// 캐릭터 초기화
	{
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
	}

	// 플레이어 총알
	{
		Model* player_bullet = new Model("/OBJ/sphere2.obj");

		for (int i = 0; i < MAX_USER_BULLET; ++i)
		{
			Entity* b = new Entity(player_bullet->GetMesh(), RGBA(1.0f, 0.0f, 0.0f, 1.0f));
			b->GetTransformComponent()->SetScale(vec3(20, 20, 20));
			b->SetVisible(false);
			BossLocker::player_bullets.push_back(b);
			Add(b);
		}
	}
	

	// 보스
	{
		m_boss = new Entity(ModelManager::GetMesh("20_idle_basic"), new Texture2D("/TEXTURE/boss_texture.png"));
		m_boss->GetTransformComponent()->SetPosition(vec3(-300, 0, 0));
		m_boss->GetTransformComponent()->SetRotation(vec3(0, 180, 0));
		m_boss->GetTransformComponent()->SetScale(vec3(100, 100, 100));
		Add(m_boss);
	}

	// 탄막
	{

		m_shooters[0]  = BulletIntervalMultipleSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[1]  = BulletBiDirectionalSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[2]  = BulletBentSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[3]  = BulletCombinedSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[4]  = BulletNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[5]  = BulletCircleNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[6]  = BulletAimingNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[7]  = BulletIntermittentAimingNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[8]  = BulletRandomNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[9] = BulletAimingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[10] = BulletLineShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[11] = BulletRandomSpreadingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[12] = BulletOvertakingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[13] = BulletSimpleHomingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[14] = BulletConstrainedHomingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[15] = BulletGapShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[16] = BulletPatternShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[17] = BulletSteppingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
		m_shooters[18] = BulletSplittingShooter::Get(m_boss->GetTransformComponent()->GetPosition());

		for (int i = 0; i <= 18; ++i)
		{
			m_shooters[i]->SetEnemyTransform(BossLocker::player->character->GetTransformComponent());
		}
	}
	
	// 카메라
	{
		SetCamera(new QuaterCamera(maths::mat4::Perspective(65.0f, 1600.0f / 900.0f, 0.1f, 1000.0f), BossLocker::player->character));
	}
}

void BossLockerLayer3D::OnTick()
{

}

void BossLockerLayer3D::OnUpdate(const utils::Timestep& ts)
{
	if (stop) return;

	chrono::duration<double> diff = chrono::high_resolution_clock::now() - start;


	for (int i = 0; i < MAX_USER; ++i)
	{
		if (!BossLocker::players[i]) continue;
		BossLocker::players[i]->character->PlayAnimation();
	}

	if (Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT) && BossLocker::player->character->GetAnimation() != ANIMATION_ROLL_BASIC)
	{
		if (BossLocker::player->alive == true)
		{

			SOCKET socket = Server::GetSocket();

			cs_packet_player_attack* packet = reinterpret_cast<cs_packet_player_attack*>(m_send_buffer);
			packet->type = CS_PLAYER_ATTACK;
			packet->size = sizeof(packet);
			packet->degree = m_degree;
			m_send_wsabuf.len = sizeof(packet);

			DWORD ioByte;

			WSASend(socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);
		}

	}

	m_bulletParticle->Update(ts);

	m_shooters[BossLocker::shooterIndex]->Update();

	float mx, mz;

	auto position = BossLocker::player->character->GetTransformComponent()->GetPosition();
	bool isRoll = BossLocker::player->character->GetAnimation() == ANIMATION_ROLL_BASIC;

	for (PoolIter i(BossLocker::bulletList); i.HasNext(); )
	{
		Bullet* bullet = static_cast<Bullet*>(i.Next());

		bullet->Update(diff.count());

		mx = bullet->GetPosition().x;
		mz = bullet->GetPosition().z;



		if (!isRoll && maths::sqrt(pow(mx - position.x, 2) + pow(mz - position.z, 2)) < 50)
		{
			bullet->alive = false;

			if (bullet->isCon == false)
			{
				cs_packet_particle_collision* packet = reinterpret_cast<cs_packet_particle_collision*>(m_send_buffer);
				packet->type = CS_PARTICLE_COLLISION;
				packet->size = sizeof(packet);
				m_send_wsabuf.len = sizeof(packet);

				DWORD ioByte;

				WSASend(m_socket, &m_send_wsabuf, 1, &m_io_flag, 0, NULL, NULL);

				bullet->isCon = true;
			}
			
		}

		for (int i = 0; i < MAX_USER; ++i)
		{
			if (!BossLocker::players[i]) continue;
			//if (BossLocker::players[i] == BossLocker::player) continue;

			auto position2 = BossLocker::players[i]->character->GetTransformComponent()->GetPosition();
			isRoll = BossLocker::players[i]->character->GetAnimation() == ANIMATION_ROLL_BASIC;

			if (!isRoll && maths::sqrt(pow(mx - position2.x, 2) + pow(mz - position2.z, 2)) < 50)
			{
				bullet->alive = false;
			}
		}

		if (!bullet->alive)
		{
			i.Remove();
		}
	}

	start = chrono::high_resolution_clock::now();

	mat4 vp = GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix();
	m_skyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));
}

void BossLockerLayer3D::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&BossLockerLayer3D::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&BossLockerLayer3D::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&BossLockerLayer3D::OnMousePressedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&BossLockerLayer3D::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&BossLockerLayer3D::OnMouseReleasedEvent));
}

bool BossLockerLayer3D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	return false;
}

bool BossLockerLayer3D::OnKeyReleasedEvent(KeyReleasedEvent& event)
{
	return false;
}

bool BossLockerLayer3D::OnMousePressedEvent(MousePressedEvent& event)
{
	if (stop) return false;

	float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
	float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

	maths::vec2 mouse(event.GetX() * scaleX, event.GetY() * scaleY);

	return false;
}

bool BossLockerLayer3D::OnMouseReleasedEvent(MouseReleasedEvent& event)
{
	return false;
}

bool BossLockerLayer3D::OnMouseMovedEvent(MouseMovedEvent& event)
{
	if (stop) return false;

	if (BossLocker::player->alive == false) return false;

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
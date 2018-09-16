#include "ParticleLayer3D.h"
#include "FinalRoom2DLayer.h"

ParticleLayer3D::ParticleLayer3D()
{

}

ParticleLayer3D::~ParticleLayer3D()
{

}

void ParticleLayer3D::ParticleLayer3D::OnInit(Renderer3D& renderer)
{
	

	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.3, 0.3, 0.3), 2, vec4(1.f, 1.f, 1.f, 1.f));
	lights->Add(light);
	PushLightSetup(lights);

	start = chrono::high_resolution_clock::now();

	MouseLayer2D::SetCursor("attack");

	m_particle = new ParticleSystem(new directx::Texture2D("/TEXTURE/particle2.png"));
	m_particle->SetPosition(vec3(-750, 0, -400));
	
	Add(m_particle);

	m_bulletParticle = new BulletParticle(new directx::Texture2D("/TEXTURE/particle.png"));
	Add(m_bulletParticle);

	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	AddStatic(xAxis);
	AddStatic(yAxis);
	AddStatic(zAxis);

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
	
	m_sphere = new Model("/OBJ/sphere.obj");

	switch (FinalGame::characterType)
	{
		case CHARACTER_14:
		{
			m_animations.push_back(ModelManager::GetMesh("14_idle_basic"));
			m_animations.push_back(ModelManager::GetMesh("14_dead"));
			m_animations.push_back(ModelManager::GetMesh("14_attack_basic"));
			m_animations.push_back(ModelManager::GetMesh("14_roll_basic"));
			m_animations.push_back(ModelManager::GetMesh("14_run_attack"));
			m_animations.push_back(ModelManager::GetMesh("14_run_basic"));
			m_animations.push_back(ModelManager::GetMesh("14_idle_attack"));

			m_player = new Animation3D(m_animations, new Texture2D("/TEXTURE/14_body.png"), new Texture2D("/TEXTURE/14_face.png"));
			m_player->GetTransformComponent()->SetPosition(vec3(0, 0, 0));
			m_player->GetTransformComponent()->SetRotation(vec3(0, 0, 0));
			m_player->GetTransformComponent()->SetScale(vec3(-100, 100, 100));
			m_player->SetAnimation(1);
		}
			break;
		case CHARACTER_15:
		{
			m_animations.push_back(ModelManager::GetMesh("15_idle_basic"));
			m_animations.push_back(ModelManager::GetMesh("15_dead"));
			m_animations.push_back(ModelManager::GetMesh("15_attack_basic"));
			m_animations.push_back(ModelManager::GetMesh("15_roll_basic"));
			m_animations.push_back(ModelManager::GetMesh("15_run_attack"));
			m_animations.push_back(ModelManager::GetMesh("15_run_basic"));
			m_animations.push_back(ModelManager::GetMesh("15_idle_attack"));

			m_player = new Animation3D(m_animations, new Texture2D("/TEXTURE/15_body.png"), new Texture2D("/TEXTURE/15_face.png"));
			m_player->GetTransformComponent()->SetPosition(vec3(0, 0, 0));
			m_player->GetTransformComponent()->SetRotation(vec3(0, 0, 0));
			m_player->GetTransformComponent()->SetScale(vec3(-100, 100, 100));
			m_player->SetAnimation(1);
		}
			break;
		case CHARACTER_20:
		{

			m_animations.push_back(ModelManager::GetMesh("20_idle_basic"));
			m_animations.push_back(ModelManager::GetMesh("20_dead"));
			m_animations.push_back(ModelManager::GetMesh("20_attack_basic"));
			m_animations.push_back(ModelManager::GetMesh("20_roll_basic"));
			m_animations.push_back(ModelManager::GetMesh("20_run_attack"));
			m_animations.push_back(ModelManager::GetMesh("20_run_basic"));
			m_animations.push_back(ModelManager::GetMesh("20_idle_attack"));

			m_player = new Animation3D(m_animations, new Texture2D("/TEXTURE/20_body.png"), new Texture2D("/TEXTURE/20_face.png"));
			m_player->GetTransformComponent()->SetPosition(vec3(0, 0, 0));
			m_player->GetTransformComponent()->SetRotation(vec3(0, 0, 0));
			m_player->GetTransformComponent()->SetScale(vec3(-100, 100, 100));
			m_player->SetAnimation(1);
		}
			break;
	}



	m_boss_animations.push_back(ModelManager::GetMesh("20_idle_basic"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_dead"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_attack_basic"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_roll_basic"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_run_attack"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_run_basic"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_idle_attack"));

	m_boss = new Animation3D(m_boss_animations, new Texture2D("/TEXTURE/20_body.png"), new Texture2D("/TEXTURE/20_face.png"));
	m_boss->GetTransformComponent()->SetPosition(vec3(-300, 0, 0));
	m_boss->GetTransformComponent()->SetRotation(vec3(0, 180, 0));
	m_boss->GetTransformComponent()->SetScale(vec3(100, 100, 100));

	Add(m_player);

	Add(m_boss);

	SetCamera(new QuaterCamera(maths::mat4::Perspective(65.0f, 1600.0f / 900.0f, 0.1f, 1000.0f), m_player));

	m_shooter = BulletPatternShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooter->SetEnemyTransform(m_player->GetTransformComponent());

	/*m_shooters[0] = BulletStarShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[1] = BulletSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[2] = BulletMultipleSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[3] = BulletIntervalMultipleSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[4] = BulletBiDirectionalSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[5] = BulletBentSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[6] = BulletCombinedSpiralShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[7] = BulletNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[8] = BulletCircleNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[9] = BulletBentCircleNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[10] = BulletAimingNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());*/
	m_shooters[0] = BulletIntermittentAimingNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[1] = BulletRandomNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[2] = BulletAimingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[3] = BulletLineShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[4] = BulletSpreadingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[5] = BulletRandomSpreadingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[6] = BulletOvertakingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[7] = BulletSimpleHomingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[8] = BulletConstrainedHomingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[9] = BulletGapShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[10] = BulletPatternShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[11] = BulletSteppingShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooters[12] = BulletSplittingShooter::Get(m_boss->GetTransformComponent()->GetPosition());

	for (int i = 0; i <= 12; ++i)
	{
		m_shooters[i]->SetEnemyTransform(m_player->GetTransformComponent());
	}


	std::string mapData = system::FileSystem::ReadTextFile("/JSON/MAP/map3.json");

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

			//translation *= 100;
			//translation.x += 900;

			mat4 position = mat4::Identity();

			auto a = new Model3D(name);

			//m_mapObjects.push_back(a);

			a->GetTransformComponent()->SetPosition(translation);
			a->GetTransformComponent()->Rotate(rotation);
			a->GetTransformComponent()->SetScale(scale);

			//AddStatic(a);
		}
	}

	layer2d = new FinalGameLayer2D(m_boss, m_player);
	Application::GetApplication().PushLayer2D(layer2d);
}

void ParticleLayer3D::ParticleLayer3D::OnTick()
{

}

void ParticleLayer3D::ParticleLayer3D::OnUpdate(const utils::Timestep& ts)
{
	chrono::duration<double> diff = chrono::high_resolution_clock::now() - start;

	m_player->PlayAnimation();
	m_player->Update();


	m_particle->Update(ts);
	m_bulletParticle->Update(ts);

	mat4 vp = GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));

	if (Input::IsKeyPressed(SUNNY_KEY_A))		m_player->GetTransformComponent()->Translate(vec3(-5, 0, 0));
	if (Input::IsKeyPressed(SUNNY_KEY_D))		m_player->GetTransformComponent()->Translate(vec3(5, 0, 0));
	if (Input::IsKeyPressed(SUNNY_KEY_W))		m_player->GetTransformComponent()->Translate(vec3(0, 0, 5));
	if (Input::IsKeyPressed(SUNNY_KEY_S))		m_player->GetTransformComponent()->Translate(vec3(0, 0, -5));

	if (!m_player->IsRoll())
	{
		if (!Input::IsKeyPressed(SUNNY_KEY_A) && !Input::IsKeyPressed(SUNNY_KEY_D) && !Input::IsKeyPressed(SUNNY_KEY_W) && !Input::IsKeyPressed(SUNNY_KEY_S))
		{
			m_player->SetAnimation(Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT) == true ? 2 : 0);
		}
		else
		{
			m_player->SetAnimation(Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT) == true ? 4 : 5);
		}
	}
	

	m_shooters[m_shooterIndex]->Update();

	float mx, mz;

	for (PoolIter i(BossLocker::bulletList); i.HasNext(); ) 
	{
		Bullet* bullet = static_cast<Bullet*>(i.Next());
		
		bullet->Update(diff.count());

		mx = bullet->GetPosition().x;
		mz = bullet->GetPosition().z;

		if (!m_player->IsRoll() && maths::sqrt( pow(mx - m_player->GetTransformComponent()->GetPosition().x, 2) + pow(mz - m_player->GetTransformComponent()->GetPosition().z, 2 )) < 50)
		{
			bullet->alive = false;
			//m_player->hp -= 1;
		}

	

		if (!bullet->alive) i.Remove();
	}

	for (auto& b : m_bullets)
	{
		

		b->Update();
	}

	if (Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT) && m_player->Attack())
	{
		float x = m_player->GetTransformComponent()->GetPosition().x;
		float z = m_player->GetTransformComponent()->GetPosition().z;
		BulletM* b = new BulletM(m_sphere->GetMesh(), m_radian, m_boss);
		b->GetTransformComponent()->SetPosition({ x, 100, z });
		m_bullets.push_back(b);
		Add(b);
	}

	start = chrono::high_resolution_clock::now();

	if (m_player->hp <= 0)
	{
		delete Application::GetApplication().PopLayer(this);
		delete Application::GetApplication().PopLayer(layer2d);
		Application::GetApplication().PushLayer2D(new FinalRoom2DLayer());
	}
}

void ParticleLayer3D::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&ParticleLayer3D::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&ParticleLayer3D::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&ParticleLayer3D::OnMousePressedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&ParticleLayer3D::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&ParticleLayer3D::OnMouseReleasedEvent));
}

bool ParticleLayer3D::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (event.GetKeyCode() == SUNNY_KEY_1)
	{
		m_shooterIndex++;
		if (m_shooterIndex == 24)
			m_shooterIndex = 0;
	}

	switch (event.GetKeyCode())
	{
		case SUNNY_KEY_1:
		{
			m_shooterIndex++;
			if (m_shooterIndex == 13)
				m_shooterIndex = 0;
		}
		break;
	case SUNNY_KEY_SPACE:
		{
			m_player->SetAnimation(3);
			m_player->SetRoll(true);
		}

	case SUNNY_KEY_3:
		{
			//delete Application::GetApplication().PopLayer(this);
			//delete layer2d;
			//Application::GetApplication().PushLayer2D(new FinalRoom2DLayer());
	}
		break;
	}
	

	return false;
}

bool ParticleLayer3D::OnKeyReleasedEvent(KeyReleasedEvent& event)
{
	return false;
}

bool ParticleLayer3D::OnMousePressedEvent(MousePressedEvent& event)
{
	return false;
}

bool ParticleLayer3D::OnMouseReleasedEvent(MouseReleasedEvent& event)
{
	return false;
}

bool ParticleLayer3D::OnMouseMovedEvent(MouseMovedEvent& event)
{
	/**maths::vec2 mouse_xy(event.GetX(), Window::GetWindowClass()->GetHeight() - event.GetY());

	vec3 viewProjection = (GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix() * m_player->GetTransformComponent()->GetTransform()).GetPosition();
	double x = viewProjection.x / viewProjection.z;
	double y = viewProjection.y / viewProjection.z;
	double z = viewProjection.z / viewProjection.z;

	double screenX = x * (Window::GetWindowClass()->GetWidth() / 2.0f) + 0 + (Window::GetWindowClass()->GetWidth() / 2.0f);
	double screenY = y * (Window::GetWindowClass()->GetHeight() / 2.0f) + 0 + (Window::GetWindowClass()->GetHeight() / 2.0f);

	float radian = maths::atan2(mouse_xy.y - screenY, mouse_xy.x - screenX);
	float degree = radian * (180 / maths::SUNNY_PI);

	m_player->GetTransformComponent()->SetRotation(vec3(0, degree + 90, 0));**/

	maths::vec2 mouse_xy(event.GetX(), event.GetY());

	vec3 P = m_player->GetTransformComponent()->GetPosition();
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

	m_player->GetTransformComponent()->SetRotation({ 0, m_degree + 90.0f , 0 });

	return false;
}
#include "ParticleLayer3D.h"

ParticleLayer3D::ParticleLayer3D()
{

}

ParticleLayer3D::~ParticleLayer3D()
{

}

void ParticleLayer3D::ParticleLayer3D::OnInit(Renderer3D& renderer)
{
	start = chrono::high_resolution_clock::now();

	MouseLayer2D::SetCursor("attack");

	m_particle = new ParticleSystem(new directx::Texture2D("/TEXTURE/particle.png"));
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
	m_player->GetTransformComponent()->SetScale(vec3(100, 100, 100));

	m_boss_animations.push_back(ModelManager::GetMesh("20_idle_basic"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_dead"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_attack_basic"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_roll_basic"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_run_attack"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_run_basic"));
	m_boss_animations.push_back(ModelManager::GetMesh("20_idle_attack"));

	m_boss = new Animation3D(m_boss_animations, new Texture2D("/TEXTURE/20_body.png"), new Texture2D("/TEXTURE/20_face.png"));
	m_boss->GetTransformComponent()->SetPosition(vec3(0, 0, 1500));
	m_boss->GetTransformComponent()->SetRotation(vec3(0, 180, 0));
	m_boss->GetTransformComponent()->SetScale(vec3(100, 100, 100));

	Add(m_player);
	Add(m_boss);

	SetCamera(new QuaterCamera(maths::mat4::Perspective(65.0f, 1600.0f / 900.0f, 0.1f, 1000.0f), m_player));

	m_shooter = BulletRandomNWayShooter::Get(m_boss->GetTransformComponent()->GetPosition());
	m_shooter->SetEnemyTransform(m_player->GetTransformComponent());
}

void ParticleLayer3D::ParticleLayer3D::OnTick()
{

}

void ParticleLayer3D::ParticleLayer3D::OnUpdate(const utils::Timestep& ts)
{
	chrono::duration<double> diff = chrono::high_resolution_clock::now() - start;

	m_player->PlayAnimation();

	m_particle->Update(ts);
	m_bulletParticle->Update(ts);

	mat4 vp = GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));

	if (Input::IsKeyPressed(SUNNY_KEY_A))		m_player->GetTransformComponent()->Translate(vec3(-5, 0, 0));
	if (Input::IsKeyPressed(SUNNY_KEY_D))		m_player->GetTransformComponent()->Translate(vec3(5, 0, 0));
	if (Input::IsKeyPressed(SUNNY_KEY_W))		m_player->GetTransformComponent()->Translate(vec3(0, 0, 5));
	if (Input::IsKeyPressed(SUNNY_KEY_S))		m_player->GetTransformComponent()->Translate(vec3(0, 0, -5));

	if (!Input::IsKeyPressed(SUNNY_KEY_A) && !Input::IsKeyPressed(SUNNY_KEY_D) && !Input::IsKeyPressed(SUNNY_KEY_W) && !Input::IsKeyPressed(SUNNY_KEY_S))
		m_player->SetAnimation(0);
	else
		m_player->SetAnimation(5);

	m_shooter->Update();

	for (PoolIter i(BossLocker::bulletList); i.HasNext(); ) 
	{
		Bullet* bullet = static_cast<Bullet*>(i.Next());
		
		bullet->Update(diff.count());

		if (!bullet->alive) i.Remove();
	}

	start = chrono::high_resolution_clock::now();
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
	maths::vec2 mouse_xy(event.GetX(), Window::GetWindowClass()->GetHeight() - event.GetY());

	vec3 viewProjection = (GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix() * m_player->GetTransformComponent()->GetTransform()).GetPosition();
	double x = viewProjection.x / viewProjection.z;
	double y = viewProjection.y / viewProjection.z;
	double z = viewProjection.z / viewProjection.z;

	double screenX = x * (Window::GetWindowClass()->GetWidth() / 2.0f) + 0 + (Window::GetWindowClass()->GetWidth() / 2.0f);
	double screenY = y * (Window::GetWindowClass()->GetHeight() / 2.0f) + 0 + (Window::GetWindowClass()->GetHeight() / 2.0f);

	float radian = maths::atan2(mouse_xy.y - screenY, mouse_xy.x - screenX);
	float degree = radian * (180 / maths::SUNNY_PI);

	m_player->GetTransformComponent()->SetRotation(vec3(0, degree + 90, 0));

	return false;
}
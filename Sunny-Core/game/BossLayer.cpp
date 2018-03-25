#include "BossLayer.h"

BossLayer::BossLayer()
{

}

BossLayer::~BossLayer()
{

}

void BossLayer::OnInit(Renderer3D& renderer)
{
	// 셰이더
	ShaderManager::Add(Shader::CreateFromFile("basic_light", std::string("resource/hlsl/basic_light.hlsl")));
	ShaderManager::Add(Shader::CreateFromFile("terrain",     std::string("resource/hlsl/terrain.hlsl")    ));

	// 머터리얼, 머터리얼 인스턴스
	Material* lightMaterial = new Material(ShaderManager::Get("basic_light"));
	MaterialInstance* lightMaterialInstance = new MaterialInstance(lightMaterial);

	Material* terrainMaterial = new Material(ShaderManager::Get("terrain"));
	MaterialInstance* terrainMaterialInstance = new MaterialInstance(terrainMaterial);

	// 전역 조명 세팅
	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.5, 0.5, 0.5), 1, vec4(1.f, 1.f, 1.f, 1.f));
	lights->Add(light);
	PushLightSetup(lights);

	// 맵 세팅
	m_heightMap = new HeightMap(500, 500, "/raw/terrain2.raw");
	Model* terrain_model = new Model("/obj/terrain2.obj");
	Terrain* terrain = new Terrain(terrain_model->GetMesh(), new Texture2D("/texture/diffuse.tga"), m_heightMap);
	terrain->SetMaterial(terrainMaterialInstance);
	Add(terrain);

	// 보스몬스터 세팅
	Model* model = new Model("/sun/boss_attack2.sun");
	Texture2D* texture = new Texture2D("/texture/boss.png");
	m_entity = new Entity(model->GetMesh(), texture);
	m_entity->SetMaterial(lightMaterialInstance);
	m_entity->GetTransformComponent()->Translate({ 0, 28, 0 });
	m_entity->GetTransformComponent()->Rotate(-90.f, vec3(1, 0, 0));
	//m_entity->GetTransformComponent()->Rotate(90.f, vec3(0, 0, 1));
	m_entity->GetTransformComponent()->SetScale(vec3(0.3, 0.3, 0.3));
	Add(m_entity);


	// xyz축 세팅
	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity());
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity());
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity());
	Add(xAxis);
	Add(yAxis);
	Add(zAxis);


	//SetCamera(new FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)));
}

void BossLayer::OnTick()
{

}

void BossLayer::OnUpdate(const utils::Timestep& ts)
{
	m_entity->PlayAnimation();
	
	//std::cout << m_heightMap->GetHeight(m_entity->GetTransformComponent()->GetPosition()) << std::endl;
}

void BossLayer::SetEntity(unsigned int index)
{
	
}

void BossLayer::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&BossLayer::OnKeyPressedEvent));
}

bool BossLayer::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if (event.GetKeyCode() == SUNNY_KEY_A)		m_entity->GetTransformComponent()->Translate(vec3(-2, 0, 0));
	if (event.GetKeyCode() == SUNNY_KEY_D)		m_entity->GetTransformComponent()->Translate(vec3(2, 0, 0));
	if (event.GetKeyCode() == SUNNY_KEY_W)		m_entity->GetTransformComponent()->Translate(vec3(0, 0, -2));
	if (event.GetKeyCode() == SUNNY_KEY_S)		m_entity->GetTransformComponent()->Translate(vec3(0, 0, 2));

	//std::cout << (m_entity->GetTransformComponent()->GetPosition()) << std::endl;
	
	return false;
}
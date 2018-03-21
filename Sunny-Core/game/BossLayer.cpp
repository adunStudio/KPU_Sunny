#include "BossLayer.h"

BossLayer::BossLayer()
{

}

BossLayer::~BossLayer()
{

}

void BossLayer::OnInit(Renderer3D& renderer)
{
	ShaderManager::Add(Shader::CreateFromFile("basic_light", std::string("resource/hlsl/basic_light.hlsl")));
	
	Material* lightMaterial = new Material(ShaderManager::Get("basic_light"));
	MaterialInstance* lightMaterialInstance = new MaterialInstance(lightMaterial);



	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.5, 0.5, 0.5), 1, vec4(1.f, 1.f, 1.f, 1.f));

	lights->Add(light);

	PushLightSetup(lights);

	Model* model = new Model("/suns/boss_attack2.sun");


	Texture2D* texture = new Texture2D("/textures/boss.png");

	// TRS
	mat4 position = mat4::Identity()*mat4::Translate(vec3(0, 0, 0)) * mat4::Rotate(-90, vec3(1, 0, 0))  *mat4::Scale(vec3(0.1, 0.1, 0.1));

	m_entity = new Entity(model->GetMesh(), texture);

	m_entity->SetMaterial(lightMaterialInstance);
	m_entity->Translate({ 0, 10, 0 });
	m_entity->Rotate(-90.f, vec3(1, 0, 0));
	m_entity->Rotate(90.f, vec3(0, 0, 1));
	m_entity->SetScale(vec3(0.1, 0.1, 0.1));



	Entity* plane = new Entity(MeshFactory::CreatePlane(200, 200, vec3(0, 0, 1)), RGBA(0.1, 0.86, 0.1, 0.5), mat4::Identity());
	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity());
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity());
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity());

	Add(plane);
	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	Add(m_entity);

	//SetCamera(new FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)));
}

void BossLayer::OnTick()
{

}

void BossLayer::OnUpdate(const utils::Timestep& ts)
{
	m_entity->PlayAnimation();
	//m_entity->Rotate(1, vec3(0, 0, 1));
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


	if (event.GetKeyCode() == SUNNY_KEY_A)		m_entity->Translate(vec3(-2, 0, 0));
	if (event.GetKeyCode() == SUNNY_KEY_D)		m_entity->Translate(vec3(2, 0, 0));
	if (event.GetKeyCode() == SUNNY_KEY_W)		m_entity->Translate(vec3(0, 0, -2));
	if (event.GetKeyCode() == SUNNY_KEY_S)		m_entity->Translate(vec3(0, 0, 2));

	/*switch (event.GetKeyCode())
	{
	case SUNNY_KEY_A:
		m_entity->Translate(vec3(-2, 0, 0));
		break;
	case SUNNY_KEY_D:
		m_entity->Translate(vec3(2, 0, 0));
		break;
	case SUNNY_KEY_W:
		m_entity->Translate(vec3(0, 0, -2));
		break;
	case SUNNY_KEY_S:
		m_entity->Translate(vec3(0, 0, 2));
		break;
	}*/

	return false;
}
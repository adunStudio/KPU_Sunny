#include "NPCLayer.h"

NPCLayer::NPCLayer()
{

}

NPCLayer::~NPCLayer()
{

}

void NPCLayer::OnInit(Renderer3D& renderer)
{
	ShaderManager::Add(Shader::CreateFromFile("geometry1", std::string("resource/hlsl/geometry.hlsl")));

	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.5, 0.5, 0.5), 10, vec4(1.f, 1.f, 1.f, 1.f));
	
	lights->Add(light);
	
	PushLightSetup(lights);

	// 머터리얼, 머터리얼 인스턴스
	Material* lightMaterial = new Material(ShaderManager::Get("geometry1"));
	MaterialInstance* lightMaterialInstance = new MaterialInstance(lightMaterial);

	Model* model = new Model("/sun/npc_idle.sun");
	

	Texture2D* texture = new Texture2D("/texture/npc_idle.png");
	
	mat4 position = mat4::Identity() * mat4::Translate(vec3(0, 5, 0)) * mat4::Rotate(-90, vec3(1, 0, 0)) * mat4::Scale(vec3(0.1, 0.1, 0.1));

	m_entity = new Entity(model->GetMesh(), texture, position);
	//m_entity->SetMaterial(lightMaterialInstance);

	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity());
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity());
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity());

	Entity* a = new Entity(MeshFactory::CreateQuad(0, 0, 10, 20), new Texture2D("/texture/time.jpg"), mat4::Identity());
	Entity* b = new Entity(MeshFactory::CreateCube(20), RGBA(1.0f, 1.0f, 1.0f, 0.5f), mat4::Identity() * mat4::Translate(maths::vec3(-20, 0, 0)));
	Entity* c = new Entity(MeshFactory::CreateCube(20), RGBA(1.0f, 0.0f, 0.0f, 0.5f), mat4::Identity() * mat4::Translate(maths::vec3(20, 0, 0)));
	Entity* d = new Entity(MeshFactory::CreateCube(20), RGBA(1.0f, 0.0f, 1.0f, 0.5f), mat4::Identity() * mat4::Translate(maths::vec3(0, 0, 20)));
	Entity* e = new Entity(MeshFactory::CreateCube(20), RGBA(1.0f, 1.0f, 0.0f, 0.5f), mat4::Identity() * mat4::Translate(maths::vec3(0, 0, -20)));
	Entity* f = new Entity(MeshFactory::CreateCube(20), RGBA(0.0f, 1.0f, 1.0f, 0.5f), mat4::Identity() * mat4::Translate(maths::vec3(0, 20, 0)));
	Entity* g = new Entity(MeshFactory::CreateCube(20), RGBA(0.0f, 0.0f, 0.0f, 0.5f), mat4::Identity() * mat4::Translate(maths::vec3(0, -20, 0)));

	


	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	Add(a);
	Add(b);
	Add(c);
	Add(d);
	Add(e);
	Add(f);
	Add(g);
	Add(m_entity);

	

	//SetCamera(new FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)));
}

void NPCLayer::OnTick()
{

}

void NPCLayer::OnUpdate(const utils::Timestep& ts)
{
	m_entity->PlayAnimation();
}


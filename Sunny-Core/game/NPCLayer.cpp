#include "NPCLayer.h"

NPCLayer::NPCLayer()
{

}

NPCLayer::~NPCLayer()
{

}

void NPCLayer::OnInit(Renderer3D& renderer)
{
	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.8f));
	
	lights->Add(light);
	
	PushLightSetup(lights);


	Model* model = new Model("/suns/npc_idle.sun");
	

	Texture2D* texture = Texture2D::CreateFromFIle("/textures/npc_idle.png");
	
	mat4 position = mat4::Identity() * mat4::Translate(vec3(0, 0, 0)) * mat4::Rotate(-90, vec3(1, 0, 0)) * mat4::Scale(vec3(0.1, 0.1, 0.1));

	m_entity = new Entity(model->GetMesh(), texture, position);

	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity());
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity());
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity());

	Entity* a = new Entity(MeshFactory::CreateQuad(0, 0, 10, 20), directx::Texture2D::CreateFromFIle("/textures/time.jpg"), mat4::Identity());
	Entity* b = new Entity(MeshFactory::CreateCube(20), directx::Texture2D::CreateFromFIle("/textures/time.jpg"), mat4::Identity() * mat4::Translate(maths::vec3(-20, 0, 0)));

	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	Add(a);
	//Add(b);
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


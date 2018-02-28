#include "TestLayer3D.h"

TestLayer3D::TestLayer3D()
{

}

TestLayer3D::~TestLayer3D()
{

}

void TestLayer3D::OnInit(Renderer3D& renderer)
{
	//Model* model = new Model("/suns/boss_idle.sun");
	Model* model = new Model("/suns/npc_idle.sun");
	//Model* cube = ;
	
	Texture2D* texture = Texture2D::CreateFromFIle("/textures/npc_idle.png");
	
	// TRS
	mat4 position = mat4::Identity() * mat4::Translate(vec3(0, 0, 0)) * mat4::Rotate(-90, vec3(1, 0, 0)) * mat4::Scale(vec3(0.1, 0.1, 0.1)) ;

	
	m_entity = new Entity(model->GetMesh(), texture, position);

	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity());
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity());
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity());

	Entity* a = new Entity(MeshFactory::CreateQuad(0, 0, 10, 10), directx::Texture2D::CreateFromFIle("/textures/time.png"), mat4::Identity() );

	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	Add(a);
	Add(m_entity);
	
	SetCamera(new FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)));
}

void TestLayer3D::OnTick()
{

}

void TestLayer3D::OnUpdate(const utils::Timestep& ts)
{
	m_entity->PlayAnimation();
}


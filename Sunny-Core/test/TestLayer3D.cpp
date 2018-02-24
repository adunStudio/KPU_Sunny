#include "TestLayer3D.h"

TestLayer3D::TestLayer3D()
{

}

TestLayer3D::~TestLayer3D()
{

}

void TestLayer3D::OnInit(Renderer3D& renderer)
{
	Model* model = new Model("/suns/boss_idle.sun");

	Texture2D* texture = Texture2D::CreateFromFIle("/textures/boss.png");
	
	mat4 position = mat4::Identity() * mat4::Scale(vec3(0.1, 0.1, 0.1)) * mat4::Rotate(45, vec3(0, 1, 0));

	
	Entity* entity  = new Entity(model->GetMesh(), texture, position);


	Add(entity);
}

void TestLayer3D::OnTick()
{

}

void TestLayer3D::OnUpdate(const utils::Timestep& ts)
{
}


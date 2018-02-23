#include "TestLayer3D.h"

TestLayer3D::TestLayer3D()
{

}

TestLayer3D::~TestLayer3D()
{

}

void TestLayer3D::OnInit(Renderer3D& renderer)
{
	Model* model = new Model("/objs/stone.obj");
	Texture2D* texture = Texture2D::CreateFromFIle("/textures/npc_idle.png");
	//mat4 position = mat4::Identity() * mat4::Rotate(180, vec3(0, 0, 1)) *  mat4::Scale(vec3(0.1, 0.1, 0.1));
	mat4 position = mat4::Identity() * mat4::Scale(vec3(0.5, 0.5, 0.5));


	Entity* entity  = new Entity(model->GetMesh(), RGBA(0.5, 0.5, 0.5, 1), position);


	Add(entity);
}

void TestLayer3D::OnTick()
{

}

void TestLayer3D::OnUpdate(const utils::Timestep& ts)
{
}


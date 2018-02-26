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
	//Model* cube = ;
	
	Texture2D* texture = Texture2D::CreateFromFIle("/textures/boss.png");
	
	mat4 position = mat4::Identity() *mat4::Scale(vec3(0.1, 0.1, 0.1));// * // *mat4::Translate(vec3(100, 0, 0));// *mat4::Rotate(45, vec3(0, 1, 0));

	
	Entity* entity = new Entity(model->GetMesh(), texture, position);
//	Entity* entity  = new Entity(MeshFactory::CreateCube(10), RGBA(1, 0, 0, 1), position);


	Add(entity);

	//SetCamera(new FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)));
}

void TestLayer3D::OnTick()
{

}

void TestLayer3D::OnUpdate(const utils::Timestep& ts)
{
}


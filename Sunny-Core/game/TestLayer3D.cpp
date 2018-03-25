#include "TestLayer3D.h"

TestLayer3D::TestLayer3D()
{

}

TestLayer3D::~TestLayer3D()
{

}

void TestLayer3D::OnInit(Renderer3D& renderer)
{
	ShaderManager::Add(Shader::CreateFromFile("terrain", std::string("resource/hlsl/terrain.hlsl")));

	Material* terrainMaterial = new Material(ShaderManager::Get("terrain"));
	MaterialInstance* terrainMaterialInstance = new MaterialInstance(terrainMaterial);

	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.5, 0.5, 0.5), 1, vec4(1.f, 1.f, 1.f, 1.f));

	lights->Add(light);
	PushLightSetup(lights);

	Model* terrain_model = new Model("/obj/terrain2.obj");

	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity());
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity());
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity());

	HeightMap* heightMap = new HeightMap(500, 500, "/raw/terrain2.raw");

	Terrain* terrain = new Terrain(terrain_model->GetMesh(), new Texture2D("/texture/diffuse.tga"), heightMap);
	terrain->SetMaterial(terrainMaterialInstance);

	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	Add(terrain);

	//SetCamera(new FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)));
}

void TestLayer3D::OnTick()
{

}

void TestLayer3D::OnUpdate(const utils::Timestep& ts)
{
}


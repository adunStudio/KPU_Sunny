#include "TestLayer3D.h"

TestLayer3D::TestLayer3D()
{

}

TestLayer3D::~TestLayer3D()
{

}

void TestLayer3D::OnInit(Renderer3D& renderer)
{

	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.5, 0.5, 0.5), 10, vec4(1.f, 1.f, 1.f, 1.f));

	lights->Add(light);

	PushLightSetup(lights);

	Entity* tree1 = new Entity(ModelManager::GetMesh("DeadOak1"), TextureManager::Get("DeadOakTreeTrunk"));
	
	Model* tree2_model = new Model("/SUN/Trees/SpruceTree1.sun");
	Entity* tree2 = new Entity(tree2_model->GetMesh(), new Texture2D("/TEXTURE/Trees/SpruceTreeLeaf.png"), new Texture2D("/TEXTURE/Trees/SpruceTreeTrunk.png"), mat4::Identity() * mat4::Translate(vec3(-10, 0, 0)));

	Add(tree1);
	Add(tree2);
}

void TestLayer3D::OnTick()
{

}

void TestLayer3D::OnUpdate(const utils::Timestep& ts)
{
}

void TestLayer3D::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&TestLayer3D::OnKeyPressedEvent));
}

bool TestLayer3D::OnKeyPressedEvent(KeyPressedEvent& event)
{

	if (event.GetKeyCode() == SUNNY_KEY_1 && !event.GetRepeat()) Renderer3D::DEFERRED_MODE = !Renderer3D::DEFERRED_MODE;

	return false;
}
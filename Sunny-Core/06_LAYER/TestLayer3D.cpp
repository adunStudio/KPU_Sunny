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
	Light* light = new Light(vec3(0.3, 0.3, 0.3), 10, vec4(1.f, 1.f, 1.f, 1.f));

	lights->Add(light);

	PushLightSetup(lights);

	Tree* tree1  = new Tree("DeadOak1",    mat4::Identity() * mat4::Translate(vec3(-50, 0, 0)));
	Tree* tree2  = new Tree("DeadOak2",    mat4::Identity() * mat4::Translate(vec3(-40, 0, 0)));
	Tree* tree3  = new Tree("DeadOak2",    mat4::Identity() * mat4::Translate(vec3(-30, 0, 0)));
	Tree* tree4  = new Tree("DeadSpruce1", mat4::Identity() * mat4::Translate(vec3(-20, 0, 0)));
	Tree* tree5  = new Tree("DeadSpruce2", mat4::Identity() * mat4::Translate(vec3(-10, 0, 0)));
	Tree* tree6  = new Tree("DeadSpruce3", mat4::Identity() * mat4::Translate(vec3(  0, 0, 0)));
	Tree* tree7  = new Tree("OakTree1",    mat4::Identity() * mat4::Translate(vec3( 10, 0, 0)));
	Tree* tree8  = new Tree("OakTree2",    mat4::Identity() * mat4::Translate(vec3( 20, 0, 0)));
	Tree* tree9  = new Tree("OakTree3",    mat4::Identity() * mat4::Translate(vec3( 30, 0, 0)));
	Tree* tree10 = new Tree("SpruceTree1", mat4::Identity() * mat4::Translate(vec3( 40, 0, 0)));
	Tree* tree11 = new Tree("SpruceTree2", mat4::Identity() * mat4::Translate(vec3( 50, 0, 0)));
	Tree* tree12 = new Tree("SpruceTree3", mat4::Identity() * mat4::Translate(vec3( 60, 0, 0)));

	//Model* model = new Model("/OBJ/sphere.obj");
	Model* model2 = new Model("/OBJ/cube.obj");
	//Model* model3 = new Model("/SUN/npc_idle.sun");
	//Model* model4 = new Model("/SUN/boss_idle.sun");

	//Entity* entity = new Entity(model->GetMesh(), RGBA(0.7, 0.2, 0, 1), mat4::Identity() * mat4::Translate(vec3(100, 0, 0)));
	Entity* entity2 = new Entity(model2->GetMesh(), RGBA(0.7, 0.2, 0, 1), mat4::Identity() * mat4::Translate(vec3(-70, 0, 0)));
	//Entity* entity3 = new Entity(model3->GetMesh(), new Texture2D("/TEXTURE/npc_idle.png"), mat4::Identity() * mat4::Translate(vec3(5, 0, 0)) * mat4::Rotate(-90, vec3(1, 0, 0)) * mat4::Scale(vec3(0.3, 0.3, 0.3)));
	//Entity* entity4 = new Entity(model4->GetMesh(), new Texture2D("/TEXTURE/boss.png"), mat4::Identity() * mat4::Translate(vec3(15, 50, 0)) * mat4::Rotate(-90, vec3(1, 0, 0)) * mat4::Scale(vec3(0.3, 0.3, 0.3)));
	//tree12->GetTransformComponent()->SetScale({12.0f, 12.0f, 12.0f});
	//Add(entity);
	Add(entity2);
	//Add(entity3);
	//Add(entity4);

	Add(tree1);
	Add(tree2);
	Add(tree3);
	Add(tree4);
	Add(tree5);
	Add(tree6);
	Add(tree7);
	Add(tree8);
	Add(tree9);
	Add(tree10);
	Add(tree11);
	Add(tree12);
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
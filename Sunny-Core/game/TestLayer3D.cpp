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

	Model* model = new Model("/obj/sphere.obj");


	mat4 position = mat4::Identity();// *mat4::Translate(vec3(0, 5, 0)) * mat4::Rotate(-90, vec3(1, 0, 0)) * mat4::Scale(vec3(0.1, 0.1, 0.1));

	m_entity = new Entity(model->GetMesh(), RGBA(1, 0, 0, 1), position);
	
	Add(m_entity);



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
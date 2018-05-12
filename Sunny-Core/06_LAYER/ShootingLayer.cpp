#include "ShootingLayer.h"



ShootingLayer::ShootingLayer()
{
}


ShootingLayer::~ShootingLayer()
{
}

void ShootingLayer::OnInit(Renderer3D & renderer)
{
	LightSetup* lights = new LightSetup();
	Light* light = new Light(vec3(0.3, 0.3, 0.3), 2, vec4(1.f, 1.f, 1.f, 1.f));
	lights->Add(light);
	PushLightSetup(lights);

	m_sphere = new Model("/OBJ/sphere.obj");
	
	//BulletSystem* newSystem = new BiDirectionalSystem(m_sphere, (Layer3D*)this);
	//BulletSystem* newSystem = new PatternNWAYSystem(m_sphere, (Layer3D*)this);
	m_bulletSystem2 = new BulletGenerator2(m_sphere, (Layer3D*)this);
	m_bulletSystem9 = new BulletGenerator9(m_sphere, (Layer3D*)this);
	m_bulletSystem3 = new BulletGenerator3(m_sphere, (Layer3D*)this);
	start = chrono::high_resolution_clock::now();
}

void ShootingLayer::OnTick()
{
}

void ShootingLayer::OnUpdate(const utils::Timestep & ts)
{
	chrono::duration<double> diff = chrono::high_resolution_clock::now() - start;
	//m_bulletSystem2->Update(diff.count());
	//m_bulletSystem9->Update(diff.count());
	m_bulletSystem3->Update(diff.count());
	start = chrono::high_resolution_clock::now();
	
}

void ShootingLayer::OnEvent(Event & event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&ShootingLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&ShootingLayer::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&ShootingLayer::OnMousePressedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&ShootingLayer::OnMouseReleasedEvent));
}

bool ShootingLayer::OnKeyPressedEvent(KeyPressedEvent & event)
{

	//if (event.GetKeyCode() == SUNNY_KEY_W) GetCamera()->Translate(vec3(0, 0, 10));
	//if (event.GetKeyCode() == SUNNY_KEY_S) GetCamera()->Translate(vec3(0, 0, -10));
	//if (event.GetKeyCode() == SUNNY_KEY_A) GetCamera()->Translate(vec3(-10, 0, 0));
	//if (event.GetKeyCode() == SUNNY_KEY_D) GetCamera()->Translate(vec3(10, 0, 0));
	

	return false;
}

bool ShootingLayer::OnKeyReleasedEvent(KeyReleasedEvent & event)
{
	return false;
}

bool ShootingLayer::OnMousePressedEvent(MousePressedEvent & event)
{
	return false;
}

bool ShootingLayer::OnMouseReleasedEvent(MouseReleasedEvent & event)
{
	return false;
}

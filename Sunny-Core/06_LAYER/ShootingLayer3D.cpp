#include "ShootingLaye3D.h"



ShootingLayer3D::ShootingLayer3D()
{
}


ShootingLayer3D::~ShootingLayer3D()
{
}

void ShootingLayer3D::OnInit(Renderer3D & renderer)
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
	m_bulletSystem11 = new BulletGenerator11(m_sphere, (Layer3D*)this);
	m_bulletSystem1 = new BulletGenerator1(m_sphere, (Layer3D*)this);
	start = chrono::high_resolution_clock::now();
}

void ShootingLayer3D::OnTick()
{
}

void ShootingLayer3D::OnUpdate(const utils::Timestep & ts)
{
	chrono::duration<double> diff = chrono::high_resolution_clock::now() - start;
	//m_bulletSystem2->Update(diff.count());
	m_bulletSystem1->Update(diff.count());
	start = chrono::high_resolution_clock::now();

}

void ShootingLayer3D::OnEvent(Event & event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&ShootingLayer3D::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&ShootingLayer3D::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&ShootingLayer3D::OnMousePressedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&ShootingLayer3D::OnMouseReleasedEvent));
}

bool ShootingLayer3D::OnKeyPressedEvent(KeyPressedEvent & event)
{

	//if (event.GetKeyCode() == SUNNY_KEY_W) GetCamera()->Translate(vec3(0, 0, 10));
	//if (event.GetKeyCode() == SUNNY_KEY_S) GetCamera()->Translate(vec3(0, 0, -10));
	//if (event.GetKeyCode() == SUNNY_KEY_A) GetCamera()->Translate(vec3(-10, 0, 0));
	//if (event.GetKeyCode() == SUNNY_KEY_D) GetCamera()->Translate(vec3(10, 0, 0));


	return false;
}

bool ShootingLayer3D::OnKeyReleasedEvent(KeyReleasedEvent & event)
{
	return false;
}

bool ShootingLayer3D::OnMousePressedEvent(MousePressedEvent & event)
{
	return false;
}

bool ShootingLayer3D::OnMouseReleasedEvent(MouseReleasedEvent & event)
{
	return false;
}

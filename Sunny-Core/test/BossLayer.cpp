#include "BossLayer.h"

BossLayer::BossLayer()
{

}

BossLayer::~BossLayer()
{

}

void BossLayer::OnInit(Renderer3D& renderer)
{
	Model* model1 = new Model("/suns/boss_attack2.sun");
	Model* model2 = new Model("/suns/boss_attack1.sun");
	Model* model3 = new Model("/suns/boss_attack2.sun");
	Model* model4 = new Model("/suns/boss_attack3.sun");

	Texture2D* texture = Texture2D::CreateFromFIle("/textures/boss.png");

	// TRS
	mat4 position = mat4::Identity()*mat4::Translate(vec3(0, 0, 0)) * mat4::Rotate(-90, vec3(1, 0, 0)) *mat4::Scale(vec3(0.1, 0.1, 0.1));

	Entity* e1 = new Entity(model1->GetMesh(), texture);
	e1->Translate({ 0, 0, 0 });
	e1->Rotate(-90.f, vec3(1, 0, 0));
	e1->Scale(vec3(0.1, 0.1, 0.1));

	//Entity* e1 = new Entity(model1->GetMesh(), RGBA(1,1,0, 1), position);
	Entity* e2 = new Entity(model2->GetMesh(), texture, position);
	Entity* e3 = new Entity(model3->GetMesh(), texture, position);
	Entity* e4 = new Entity(model4->GetMesh(), texture, position);

	m_entity = e1;

	entities.push_back(e1);
	entities.push_back(e2);
	entities.push_back(e3);
	entities.push_back(e4);



	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity());
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity());
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity());


	Add(xAxis);
	Add(yAxis);
	Add(zAxis);

	Add(m_entity);

	//SetCamera(new FPSCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)));
}

void BossLayer::OnTick()
{

}

void BossLayer::OnUpdate(const utils::Timestep& ts)
{
	m_entity->PlayAnimation();
	//m_entity->Rotate(1, vec3(0, 0, 1));
}

void BossLayer::SetEntity(unsigned int index)
{
	
}

void BossLayer::OnEvent(Event& event)
{
	Layer3D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&BossLayer::OnKeyPressedEvent));
}

bool BossLayer::OnKeyPressedEvent(KeyPressedEvent& event)
{
	switch (event.GetKeyCode())
	{
	case SUNNY_KEY_LEFT:
		m_entity->Translate(vec3(-2, 0, 0));
		break;
	case SUNNY_KEY_RIGHT:
		m_entity->Translate(vec3(2, 0, 0));
		break;
	case SUNNY_KEY_UP:
		m_entity->Translate(vec3(0, 0, -2));
		break;
	case SUNNY_KEY_DOWN:
		m_entity->Translate(vec3(0, 0, 2));
		break;
	}

	return false;
}
#include "FinalGameLayer2D.h"

FinalGameLayer2D::FinalGameLayer2D(Animation3D* boss, Animation3D* player)
	: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
	, m_boss(boss), m_player(player)
{
	
}

FinalGameLayer2D::~FinalGameLayer2D()
{
	//delete m_panel;
	Application::GetApplication().PopLayer(this);
}

void FinalGameLayer2D::OnInit(Renderer2D& renderer)
{
	m_panel = new Panel();
	
	m_bossHPBar = new Progressbar(maths::Rectangle(300, 830, 500, 20));
	m_bossHPBar->SetValue(1);
	m_panel->Add(m_bossHPBar);

	m_playerHPBar = new Progressbar(maths::Rectangle(0, 0, 200, 20));
	m_playerHPBar->SetValue(1);
	m_panel->Add(m_playerHPBar);
}

void FinalGameLayer2D::OnTick()
{

}

void FinalGameLayer2D::OnUpdate(const utils::Timestep& ts)
{
	float hp = m_boss->hp;
	m_bossHPBar->SetValue(hp / 100.0f);

	hp = m_player->hp;
	m_playerHPBar->SetValue(hp / 100.0f);
}

void FinalGameLayer2D::OnRender(Renderer2D& renderer)
{

}

void FinalGameLayer2D::OnEvent(Event& event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&FinalGameLayer2D::OnMouseMovedEvent));
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&FinalGameLayer2D::OnKeyPressedEvent));
	dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&FinalGameLayer2D::OnMouseReleasedEvent));
}

bool FinalGameLayer2D::OnMouseMovedEvent(MouseMovedEvent& event)
{
	return false;
}

bool FinalGameLayer2D::OnMouseReleasedEvent(MouseReleasedEvent& event)
{
	
	return false;
}

bool FinalGameLayer2D::OnKeyPressedEvent(KeyPressedEvent& event)
{


	return false;
}











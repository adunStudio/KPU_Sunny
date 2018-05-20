#include "LoadingLayer2D.h"

LoadingLayer2D::LoadingLayer2D()
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{

}

LoadingLayer2D::~LoadingLayer2D()
{

}

void LoadingLayer2D::OnInit(Renderer2D& renderer)
{
	m_backgroundTexture = new Texture2D("/TEXTURE/Loadings/background.png");
	m_background = new Sprite(0, 0, m_windowWidth, m_windowHeight, m_backgroundTexture);
	Add(m_background);

	m_circleTexture = new Texture2D("/TEXTURE/Loadings/circle.png");

	for (int i = 0; i < 13; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160, 860 - 142 * 0, 20, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 11; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160 + 80, 860 - 142 * 1, 20, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 13; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160, 860 - 142 * 2, 20, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 11; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160 + 80, 860 - 142 * 3, 20, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 13; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160, 860 - 142 * 4, 20, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 11; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160 + 80, 860 - 142 * 5, 20, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}

	for (int i = 0; i < 13; ++i)
	{
		RoadingCircle* circle = new RoadingCircle(i * 160, 860 - 142 * 6, 20, m_circleTexture);
		m_circles.push_back(circle);
		Add(circle);
	}
}

void LoadingLayer2D::OnTick()
{

}

void LoadingLayer2D::OnUpdate(const utils::Timestep& ts)
{
	for (auto circle : m_circles)
	{
		circle->Update();
	}
	//m_circle->SetSize(vec2(m_circle->GetSize().x + 1, m_circle->GetSize().y + 1));
}

void LoadingLayer2D::OnRender(Renderer2D& renderer)
{

}

void LoadingLayer2D::OnEvent(Event& event)
{

}

bool LoadingLayer2D::OnMouseMovedEvent(MouseMovedEvent& event)
{

}

bool LoadingLayer2D::OnMousePressedEvent(MousePressedEvent& event)
{

}

bool LoadingLayer2D::OnKeyPressedEvent(KeyPressedEvent& event)
{

}
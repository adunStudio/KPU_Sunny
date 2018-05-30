#include "MouseLayer2D.h"

MouseLayer2D* MouseLayer2D::instance = nullptr;

void MouseLayer2D::SetCursor(const std::string& name)
{
	instance->SetCursorInternal(name);
}

void MouseLayer2D::SetCursorInternal(const std::string& name)
{
	if (name == "cursor")
	{
		m_center = false;
		m_size = 32;
	}
	else if (name == "attack")
	{
		m_center = true;
		m_size = 16;
	}
	else
	{
		return;
	}

	m_mouse->SetTexture(m_textures[name]);
	//m_size = m_textures[name]->GetHeight();
}

MouseLayer2D::MouseLayer2D()
	: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{

	m_textures["cursor"]  = new Texture2D("/TEXTURE/cursor1.png", DIMENSION::D2);
	m_textures["attack"]  = new Texture2D("/TEXTURE/cursor2.png", DIMENSION::D2);

	m_size = m_textures["cursor"]->GetHeight();
	m_center = false;

	m_mouse = new Sprite(0, 0, m_textures["cursor"]);

	Add(m_mouse);

	instance = this;
}

MouseLayer2D::~MouseLayer2D()
{
	for (auto texture : m_textures)
		delete texture.second;

	delete m_mouse;
}

void MouseLayer2D::OnEvent(Event& event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseMovedEvent>(METHOD(&MouseLayer2D::OnMouseMovedEvent));
}

bool MouseLayer2D::OnMouseMovedEvent(MouseMovedEvent&   event)
{
	float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
	float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

	maths::vec2 mouse(event.GetX() * scaleX, (Window::GetWindowClass()->GetHeight() - event.GetY()) * scaleY);

	if(m_center)
		m_mouse->SetPosition(vec2(mouse.x - (m_size * scaleX), mouse.y - (m_size * scaleY)));
	else
		m_mouse->SetPosition(vec2(mouse.x, mouse.y - (m_size * scaleY)));

	return false;
}
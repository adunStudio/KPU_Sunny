#include "MouseLayer2D.h"

MouseLayer2D* MouseLayer2D::instance = nullptr;

MouseLayer2D::MouseLayer2D()
	: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{

	m_textures["cursor"]  = new Texture2D("/TEXTURE/cursor1.png", DIMENSION::D2);
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

	m_mouse->SetPosition(vec2(mouse.x, mouse.y - (32 * scaleY)));

	return false;
}
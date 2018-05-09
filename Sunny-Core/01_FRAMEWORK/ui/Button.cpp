#include "Button.h"

#include "../graphics/fonts/FontManager.h"

namespace sunny
{
	namespace ui
	{
		Button::Button(const std::string& label, const maths::Rectangle& bounds, const ActionHandler handler)
		: Widget(bounds), m_sprite(nullptr), m_label(label), m_actionHandler(handler), m_state(ButtonState::UNPRESSED)
		{
			m_font = FontManager::Get(16);
		}

		Button::Button(Sprite* sprite, const ActionHandler handler)
		: Widget(sprite->GetBounds()), m_sprite(sprite), m_label(""), m_actionHandler(handler), m_state(ButtonState::UNPRESSED)
		{
			m_font = FontManager::Get(16);
		}

		Button::~Button()
		{
			if (m_sprite)
				delete m_sprite;
		}

		bool Button::OnMousePressed(events::MousePressedEvent& e)
		{
			m_state = ButtonState::PRESSED;

			return true;
		}

		bool Button::OnMouseReleased(events::MouseReleasedEvent& e)
		{
			if (m_state == ButtonState::PRESSED)
				OnAction();

			m_state = ButtonState::UNPRESSED;

			return true;
		}

		bool Button::OnMouseMoved(events::MouseMovedEvent& e)
		{
			float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
			float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

			maths::vec2 mouse(e.GetX() * scaleX, (Window::GetWindowClass()->GetHeight() - e.GetY()) * scaleY);

			if (m_state == ButtonState::PRESSED && !m_bounds.Contains(mouse))
				m_state = ButtonState::UNPRESSED;

			return false;
		}

		void Button::OnAction()
		{
			m_actionHandler();
		}

		void Button::OnUpdate()
		{
		}

		void Button::OnRender(Renderer2D& renderer)
		{
			if(m_sprite)
				renderer.Submit(m_sprite);
			else
			{
				renderer.DrawRect(m_bounds);
				renderer.FillRect(m_bounds, m_state == ButtonState::PRESSED ? RGBA(0.81f, 0.73f, 0.94f, 1) : RGBA(0.81f, 0.37f, 0.37f, 1));
				renderer.DrawString(m_label, m_bounds.position - maths::vec2(m_bounds.width, m_font->GetHeight(m_label) * 0.5f), *m_font);
			}
		}
	}
}
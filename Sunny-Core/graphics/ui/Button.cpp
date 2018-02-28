#include "Button.h"

#include "../fonts/FontManager.h"

namespace sunny
{
	namespace graphics
	{
		namespace ui
		{
			Button::Button(const std::string& label, const maths::Rectangle& bounds, const ActionHandler handler)
			: Widget(bounds), m_label(label), m_actionHandler(handler), m_state(ButtonState::UNPRESSED)
			{
				m_font = FontManager::Get();
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
				maths::vec2 mouse(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()), 18.0f - e.GetY() * (18.0f / Window::GetWindowClass(nullptr)->GetHeight()));
			
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
				renderer.DrawRect(m_bounds);
				renderer.FillRect(m_bounds, m_state == ButtonState::PRESSED ? RGBA(0.81f, 0.73f, 0.94f, 1) : RGBA(0.81f, 0.37f, 0.37f, 1));
			}
		}
	}
}
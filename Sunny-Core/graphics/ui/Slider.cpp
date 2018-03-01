#include "Slider.h"

namespace sunny
{
	namespace graphics
	{
		namespace ui
		{
			Slider::Slider(const maths::Rectangle bounds, bool vertical)
			: Widget(bounds), m_value(0.0f), m_state(SliderState::UNPRESSED), m_headOffset(0.0f), m_callback(&Slider::NoCallback), m_vertical(vertical)
			{
				float size = vertical ? bounds.width : bounds.height;
				m_headBounds = maths::Rectangle(bounds.x, bounds.y, size, size);
			}

			Slider::Slider(const maths::Rectangle bounds, float value, const ValueChangedCallback& callback, bool vertical)
				: Widget(bounds), m_value(value), m_state(SliderState::UNPRESSED), m_headOffset(0.0f),  m_callback(callback), m_vertical(vertical)
			{
				float size = vertical ? bounds.width : bounds.height;
				m_headBounds = maths::Rectangle(bounds.x, bounds.y, size, size);
			}

			bool Slider::OnMousePressed(events::MousePressedEvent&  e)
			{
				maths::vec2 mouse(e.GetX(), Window::GetWindowClass(nullptr)->GetHeight() - e.GetY());

				if (m_headBounds.Contains(mouse))
				{
					m_state = SliderState::PRESSEDHEAD;
					m_headOffset = m_vertical ? (mouse.y - m_headBounds.y) : (mouse.x - m_headBounds.x);
				}
				else
				{
					m_state = SliderState::PRESSED;
				}

				return true;
			}

			bool Slider::OnMouseReleased(events::MouseReleasedEvent& e)
			{
				m_state = SliderState::UNPRESSED;

				return true;
			}

			bool Slider::OnMouseMoved(events::MouseMovedEvent&    e)
			{
				maths::vec2 mouse(e.GetX(), Window::GetWindowClass(nullptr)->GetHeight() - e.GetY());

				
				if (m_state == SliderState::PRESSEDHEAD)
				{
					if (m_vertical)
						SetValue((mouse.y - m_bounds.GetMinimumBound().y - m_headOffset) / (m_bounds.GetMaximumBound().y));
					else
						SetValue((mouse.x - m_bounds.GetMinimumBound().x - m_headOffset) / (m_bounds.GetMaximumBound().x));
				}

				return true;
			}

			void Slider::OnUpdate()
			{
				if (!Input::IsMouseButtonPressed(SUNNY_MOUSE_LEFT))
					m_state = SliderState::UNPRESSED;

				if (m_vertical)
					m_headBounds.y = m_bounds.GetMinimumBound().y + m_headBounds.height + m_value * (m_bounds.height * 2.0f - m_headBounds.height * 2.0f);
				else
					m_headBounds.x = m_bounds.GetMinimumBound().x + m_headBounds.width + m_value * (m_bounds.width * 2.0f - m_headBounds.width * 2.0f);
			}

			void Slider::OnRender(Renderer2D& renderer)
			{
				renderer.FillRect(m_bounds, RGBA(0.81, 0.49, 0.49, 1));
				renderer.DrawRect(m_bounds);

				renderer.FillRect(m_headBounds, RGBA(0.81, 0.75, 0.75, 1));
				renderer.DrawRect(m_headBounds);

				maths::vec2 offset = m_vertical ? maths::vec2(0, m_bounds.size.y) : maths::vec2(m_bounds.size.x, 0);
				renderer.DrawLine(m_bounds.position - offset, m_bounds.position + offset);
			}

			void Slider::SetValue(float value)
			{
				value = maths::clamp(value, 0.0f, 1.0f);
				m_value = value;

				m_callback(value);
			}
		}
	}
}
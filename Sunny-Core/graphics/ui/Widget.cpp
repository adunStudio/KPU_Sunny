#include "Widget.h"

namespace sunny
{
	namespace graphics
	{
		namespace ui
		{
			Widget::Widget(const maths::Rectangle& bounds)
			: m_bounds(bounds), m_active(true), m_focused(false)
			{
			}

			bool Widget::OnMousePressed(events::MousePressedEvent& e)
			{
				return false;
			}

			bool Widget::OnMouseReleased(events::MouseReleasedEvent& e)
			{
				return false;
			}

			bool Widget::OnMouseMoved(events::MouseMovedEvent& e)
			{
				return false;
			}

			void Widget::OnUpdate()
			{
			}

			void Widget::OnRender(Renderer2D& renderer)
			{
			}
		}
	}
}
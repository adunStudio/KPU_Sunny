#include "Progressbar.h"

namespace sunny
{
	namespace ui
	{
		Progressbar::Progressbar(const maths::Rectangle bounds, float value, const CompletedCallback& callback)
		: Widget(bounds), m_value(value), m_callback(callback)
		{
			float height = bounds.height;
			float width = bounds.height;

			m_progressBounds = maths::Rectangle(bounds.x, bounds.y, width, height);
			
			m_state = ProgressbarState::LOADING;
		}

		void Progressbar::OnUpdate()
		{
			if (m_value >= 1.0f)
			{
				m_state = ProgressbarState::COMPLETED;
				m_callback(m_value);
			}

			m_progressBounds.width = m_value * m_bounds.width;
			//m_progressBounds.x = m_bounds.GetMinimumBound().x + m_progressBounds.width;

		}

		void Progressbar::OnRender(Renderer2D& renderer)
		{
			renderer.FillRect(m_bounds, RGBA(0.5, 0.5, 0.5, 0.7));
			renderer.DrawRect(m_bounds, RGBA(0.9, 0.9, 0.9, 0.7));

			renderer.FillRect(m_progressBounds, RGBA(0.7, 0.7, 0.7, 0.5));
			//renderer.DrawRect(m_progressBounds, RGBA(0.9, 0.9, 0.9, 0.7));
		}

		void Progressbar::SetValue(float value)
		{
			value = maths::clamp(value, 0.0f, 1.0f);
			m_value = value;
		}
	}
}
#include "Loadingbar.h"

namespace game
{
	Loadingbar::Loadingbar(const maths::Rectangle bounds, Label* label, const vec4& boundColor, const vec4& progressColor)
	: Progressbar(bounds, 0),m_label(label), m_currentValue(0), m_boundColor(boundColor), m_progressColor(progressColor)
	{

	}

	void Loadingbar::OnUpdate()
	{
		if (m_currentValue >= 1.0f)
		{
			m_state = ProgressbarState::COMPLETED;
			m_callback(m_value);
		}

		if (m_currentValue < m_value)
		{
			m_currentValue += 0.005;
		}

		m_currentValue = min(m_value, m_currentValue);

		m_progressBounds.width = m_currentValue * m_bounds.width;
	}

	void Loadingbar::OnRender(Renderer2D& renderer)
	{
		renderer.FillRect(m_bounds, m_boundColor);
		renderer.DrawRect(m_bounds, m_boundColor);

		renderer.FillRect(m_progressBounds, m_progressColor);

		m_label->Submit(&renderer);
	}
}
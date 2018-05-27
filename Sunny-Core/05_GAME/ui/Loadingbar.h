#pragma once

#include <SUNNY.h>

namespace game
{
	class Loadingbar : public Progressbar
	{
	private:
		vec4 m_boundColor;
		vec4 m_progressColor;
		Label* m_label;

		float m_currentValue;

	public:
		Loadingbar(const maths::Rectangle bounds, Label* label, const vec4& boundColor, const vec4& progressColor);

		virtual void OnUpdate() override;
		virtual void OnRender(Renderer2D& renderer) override;
	};
}
#pragma once
#include "SUNNY.h"

namespace game
{
	class CharacterHpBar : public Progressbar
	{
	public:
		Player * m_player = nullptr;

		CharacterHpBar(const maths::Rectangle bounds, Player* player)
		: Progressbar(bounds), m_player(player)
		{

		}
		
		void OnUpdate() override
		{
			float hp = m_player->hp;
			float max = 100.0f
				;
			SetValue(hp / max);

			/*if (m_value >= 1.0f)
			{
				m_state = ProgressbarState::COMPLETED;
				m_callback(m_value);
			}*/

			m_progressBounds.width = m_value * m_bounds.width;
			//m_progressBounds.x = m_bounds.GetMinimumBound().x + m_progressBounds.width;
		}

		void OnRender(Renderer2D& renderer) override
		{
			renderer.FillRect(m_bounds, RGBA(0.5, 0.5, 0.5, 0.7));
			renderer.DrawRect(m_bounds, RGBA(0.9, 0.9, 0.9, 0.7));

			renderer.FillRect(m_progressBounds, RGBA(1, 0.2, 0.1, 0.7));
		}
	};
}

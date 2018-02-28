#pragma once

#include "Panel.h"
#include "../../maths/maths.h"

namespace sunny
{
	namespace graphics
	{
		namespace ui
		{
			class Widget
			{
			protected:
				bool m_active;
				bool m_focused;

				Panel* m_panel;
				maths::Rectangle m_bounds;

			private:
				Widget() {};

			protected:
				Widget(const maths::Rectangle& bounds);

			public:
				virtual bool OnMousePressed (events::MousePressedEvent&  e);
				virtual bool OnMouseReleased(events::MouseReleasedEvent& e);
				virtual bool OnMouseMoved   (events::MouseMovedEvent&    e);
			
				virtual void OnUpdate();
				virtual void OnRender(Renderer2D& renderer);

				inline const maths::Rectangle& GetBounds() const { return m_bounds; }
				inline maths::Rectangle& GetBounds() { return m_bounds; }
				inline void SetBounds(const maths::Rectangle& bounds) { m_bounds = bounds; }

				inline bool IsActive() const { return m_active; }
				inline void SetActive(bool active) { m_active = active; }
			};
		}
	}
}
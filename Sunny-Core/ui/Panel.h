#pragma once

#include "../sunny.h"
#include "../graphics/layers/Layer2D.h"

namespace sunny
{
	namespace ui
	{
		using namespace graphics;

		class Widget;

		class Panel : public Layer2D
		{
		private:
			std::vector<Widget*> m_widgets;

		public:
			Panel();
			~Panel();

			Widget* Add(Widget* widget);
			void Remove(Widget* widget);
			void Clear();

			void OnUpdate(const utils::Timestep& ts) override;
			void OnRender(Renderer2D& renderer) override;

			inline const std::vector<Widget*>& GetWidgets() const { return m_widgets; }

			void OnEvent(events::Event& e) override;

			bool OnMousePressedEvent(events::MousePressedEvent&  e);
			bool OnMouseReleasedEvent(events::MouseReleasedEvent& e);
			bool OnMouseMovedEvent(events::MouseMovedEvent&    e);
		};
	}
}
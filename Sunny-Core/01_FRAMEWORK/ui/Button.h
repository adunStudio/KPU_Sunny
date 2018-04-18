#pragma once

#include "../include.h"
#include "Widget.h"

namespace sunny
{
	namespace ui
	{
		using namespace graphics;

		class Button : public Widget
		{
		public:
			typedef std::function<void()> ActionHandler;

		private:
			enum class ButtonState
			{
				PRESSED, UNPRESSED
			};

		protected:
			std::string m_label;

			ButtonState m_state;

			ActionHandler m_actionHandler;

			Font* m_font;

		public:
			Button(const std::string& label, const maths::Rectangle& bounds, const ActionHandler handler = &Button::NoAction);

			virtual bool OnMousePressed(events::MousePressedEvent&  e) override;
			virtual bool OnMouseReleased(events::MouseReleasedEvent& e) override;
			virtual bool OnMouseMoved(events::MouseMovedEvent&    e) override;

			virtual void OnAction();
			virtual void OnUpdate() override;
			virtual void OnRender(Renderer2D& renderer) override;

			inline void SetLabel(const std::string& label) { m_label = label; };
			inline const std::string& GetLabel() const { return m_label; };

			inline void SetFont(Font* font) { m_font = font; };
			inline const Font& GetFont() { return *m_font; };

			inline void SetAction(const ActionHandler& action) { m_actionHandler = action; }

		private:
			static void NoAction() {};
		};
	}
}
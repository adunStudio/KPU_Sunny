#pragma once

#include "../include.h"
#include "Widget.h"

namespace sunny
{
	namespace ui
	{
		using namespace graphics;

		class Dialog : public Widget
		{
		public:
			typedef std::function<void()> CloseCallback;

		private:
			enum class DialogState
			{
				PRESSEDHEAD, PRESSEDCLOSE, UNPRESSED
			};

		private:
			maths::Rectangle m_headBounds;
			maths::Rectangle m_closeBounds;

			std::string m_headText;

			std::vector<std::string> m_texts;

			DialogState m_state;

			Font* m_font;

			float m_offsetX, m_offsetY;

			CloseCallback m_closeCallback;

		public:
			Dialog(const maths::Rectangle bounds, const std::string& headText, const std::string& text, const CloseCallback callback = &Dialog::NoCallback);
			Dialog(const maths::Rectangle bounds, const std::string& headText, const std::vector<std::string>& texts, const CloseCallback callback = &Dialog::NoCallback);

			virtual void OnClose();
			virtual bool OnMousePressed(events::MousePressedEvent&  e) override;
			virtual bool OnMouseReleased(events::MouseReleasedEvent& e) override;
			virtual bool OnMouseMoved(events::MouseMovedEvent&    e) override;

			virtual void OnUpdate() override;
			virtual void OnRender(Renderer2D& renderer) override;

			inline void SetCloseAction(const CloseCallback& callback) { m_closeCallback = callback; }

		private:
			void SetElements();

		private:
			static void NoCallback() {};
		};
	}
}
#include "Dialog.h"

namespace sunny
{
	namespace ui
	{
		Dialog::Dialog(const maths::Rectangle bounds, const std::string& headText, const std::string& text, const CloseCallback callback)
		: Widget(bounds), m_state(DialogState::UNPRESSED), m_headText(headText), m_closeCallback(callback)
		{
			
			m_texts.push_back(text);

			m_font = FontManager::Get(16);

			SetElements();
		}

		Dialog::Dialog(const maths::Rectangle bounds, const std::string& headText, const std::vector<std::string>& texts, const CloseCallback callback)
		: Widget(bounds), m_state(DialogState::UNPRESSED), m_headText(headText), m_texts(texts), m_closeCallback(callback)
		{
			m_font = FontManager::Get(16);

			SetElements();
		}

		bool Dialog::OnMousePressed(events::MousePressedEvent&  e)
		{
			float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
			float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

			maths::vec2 mouse(e.GetX() * scaleX, (Window::GetWindowClass()->GetHeight() - e.GetY()) * scaleY);

			if (m_closeBounds.Contains(mouse))
			{
				m_state = DialogState::PRESSEDCLOSE;
			}
			else if (m_headBounds.Contains(mouse))
			{
				m_offsetX = mouse.x - m_bounds.x;
				m_offsetY = mouse.y - m_bounds.y;

				m_state = DialogState::PRESSEDHEAD;
			}

			return true;
		}

		void Dialog::OnClose()
		{
			m_closeCallback();
		}

		bool Dialog::OnMouseReleased(events::MouseReleasedEvent& e)
		{
			if (m_state == DialogState::PRESSEDCLOSE)
				OnClose();

			m_state = DialogState::UNPRESSED;

			return true;
		}

		bool Dialog::OnMouseMoved(events::MouseMovedEvent&    e)
		{
			float scaleX = Window::GetWindowClass()->GetResolutionWidth() / Window::GetWindowClass()->GetWidth();
			float scaleY = Window::GetWindowClass()->GetResolutionHeight() / Window::GetWindowClass()->GetHeight();

			maths::vec2 mouse(e.GetX() * scaleX, (Window::GetWindowClass()->GetHeight() - e.GetY()) * scaleY);

			if (m_state == DialogState::PRESSEDHEAD)
			{
				m_bounds.x = mouse.x - m_offsetX;
				m_bounds.y = mouse.y - m_offsetY;

				SetElements();
			}

			if (m_state == DialogState::PRESSEDCLOSE && !m_bounds.Contains(mouse))
				m_state = DialogState::UNPRESSED;

			return true;
		}

		void Dialog::OnUpdate()
		{

		}

		void Dialog::OnRender(Renderer2D& renderer)
		{
			renderer.DrawRect(m_bounds);
			renderer.FillRect(m_bounds, RGBA(0.81f, 0.73f, 0.94f, 1));

			renderer.FillRect(m_headBounds, RGBA(0.81, 0.75, 0.75, 1));
			renderer.DrawRect(m_headBounds);
			renderer.DrawString(m_headText, m_headBounds.position - maths::vec2(m_headBounds.width, m_font->GetHeight(m_headText) * 0.5f), *m_font, RGBA(0 ,0, 0, 1));

			renderer.FillRect(m_closeBounds, m_state == DialogState::PRESSEDCLOSE ? RGBA(0.81f, 0.37f, 0.37f, 1) : RGBA(0.81f, 0.73f, 0.94f, 1));
			renderer.DrawRect(m_closeBounds);
			renderer.DrawString("X", m_closeBounds.position - maths::vec2(m_closeBounds.width-5, m_font->GetHeight("X") * 0.5f - 1), *m_font, RGBA(0, 0, 0, 1));
		
			float height = 0;

			for (auto text : m_texts)
			{
				renderer.DrawString(text, m_headBounds.position - maths::vec2(m_headBounds.width, m_headBounds.height + 25 + height), *m_font, RGBA(0, 0, 0, 1));

				height += 25;// m_font->GetHeight(text) * 1.5f;
			}
		}

		void Dialog::SetElements()
		{
			m_headBounds = maths::Rectangle(m_bounds.x, m_bounds.GetMaximumBound().y - 23, m_bounds.width - 3, 20);
			m_closeBounds = maths::Rectangle(m_bounds.GetMaximumBound().x - 22, m_bounds.GetMaximumBound().y - 23, 11, 11);
		}
	}
}
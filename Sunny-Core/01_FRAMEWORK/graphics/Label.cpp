#include "Label.h"

#include "renderers/Renderer2D.h"
namespace sunny
{
	namespace graphics
	{
		Label::Label(const std::string& text, float x, float y, const maths::vec4 color, Alignment alignment)
		: Sprite(), m_font(FontManager::Get())
		{
			SetPosition(maths::vec3(x, y, 0.0f));
			SetText(text);
			SetAlignment(alignment);
			m_color = color;
		}

		Label::Label(const std::string& text, float x, float y, Font* font, const maths::vec4 color, Alignment alignment)
		: Sprite(), m_font(font)
		{
			SetPosition(maths::vec3(x, y, 0.0f));
			SetText(text);
			SetAlignment(alignment);
			m_color = color;
		}

		Label::Label(const std::string& text, float x, float y, const std::string& font, const maths::vec4 color, Alignment alignment)
		: Sprite(), m_font(FontManager::Get(font))
		{
			SetPosition(maths::vec3(x, y, 0.0f));
			SetText(text);
			SetAlignment(alignment);
			m_color = color;

			ValidateFont(font);
		}

		Label::Label(const std::string& text, float x, float y, const std::string& font, unsigned int size, const maths::vec4 color, Alignment alignment)
		: Sprite(), m_font(FontManager::Get(font, size))
		{
			SetPosition(maths::vec3(x, y, 0.0f));
			SetText(text);
			SetAlignment(alignment);
			m_color = color;

			ValidateFont(font);
		}

		void Label::UpdateBounds()
		{
			maths::vec2 size = m_font->GetSize(m_text) * 0.5f;

			m_bounds.size = size;

			switch (m_alignment)
			{
			case Alignment::LEFT:
				m_alignmentOffset.x = 0;  break;
			case Alignment::CENTER:
				m_alignmentOffset.x = -size.x; /* 0.0f;*/    break;
			case Alignment::RIGHT:
				m_alignmentOffset.x = -size.x * 2; break;
			}
		}

		void Label::Submit(Renderer2D* renderer) const
		{
			renderer->DrawString(m_text, m_bounds.GetMinimumBound() + m_alignmentOffset, *m_font, m_color);
		}

		void Label::ValidateFont(const std::string& name, unsigned int size)
		{
			if (m_font != nullptr) return;

			std::cout << "알수 없는 폰트입니다. (" << name;
			if (size > 0) std::cout << ", size= " << size;
			std::cout << std::endl;

			m_font = FontManager::Get();
		}

		void Label::SetAlignment(Alignment alignment)
		{
			m_alignment = alignment;

			UpdateBounds();
		}

		void Label::SetText(const std::string& text)
		{
			m_text = text;

			UpdateBounds();
		}
	}
}
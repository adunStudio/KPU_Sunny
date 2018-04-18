#pragma once

#include "../include.h"
#include "Sprite.h"
#include "fonts/FontManager.h"

namespace sunny
{
	namespace graphics
	{
		class Label : public Sprite
		{
		public:
			enum class Alignment
			{
				NONE = 0,
				LEFT,
				CENTER,
				RIGHT
			};

		private:
			Font* m_font;
			
			Alignment m_alignment;

			maths::vec2 m_alignmentOffset;

			std::string m_text;

		private:
			void UpdateBounds();

		public:
			Label(const std::string& text, float x, float y, const maths::vec4 color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& text, float x, float y, Font* font, const maths::vec4 color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& text, float x, float y, const std::string& font, const maths::vec4 color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& text, float x, float y, const std::string& font, unsigned int size, const maths::vec4 color, Alignment alignment = Alignment::LEFT);
		
			void Submit(Renderer2D* renderer) const override;

			void ValidateFont(const std::string& name, unsigned int size = -1);

			void SetAlignment(Alignment alignment);
			void SetText(const std::string& text);

			inline const Font& GetFont() const { return *m_font; }
			inline Alignment GetAligment() const { return m_alignment; }
			inline const std::string& GetText() const { return m_text; }
		};
	}
}
#include "FontManager.h"

namespace sunny
{
	namespace graphics
	{
		std::vector<Font*> FontManager::s_fonts;
		maths::vec2 FontManager::s_scale = maths::vec2(1.0f, 1.0f);
	
		void FontManager::SetScale(const maths::vec2& scale)
		{
			s_scale = scale;
		}

		void FontManager::Add(Font* font)
		{
			font->SetScale(s_scale);
			s_fonts.push_back(font);
		}

		void FontManager::Clean()
		{
			for (unsigned int i = 0; i < s_fonts.size(); ++i)
				delete s_fonts[i];
		}

		Font* FontManager::Get()
		{
			return s_fonts[0];
		}

		Font* FontManager::Get(const std::string& name)
		{
			for (Font* font : s_fonts)
				if (font->GetName() == name)
					return font;

			return nullptr;
		}

		Font* FontManager::Get(unsigned int size)
		{
			for (Font* font : s_fonts)
				if (font->GetFontSize() == size)
					return font;

			return nullptr;
		}

		Font* FontManager::Get(const std::string& name, unsigned int size)
		{
			for(Font* font : s_fonts)
				if (font->GetName() == name && font->GetFontSize() == size)
					return font;

			return nullptr;
		}
	}
}
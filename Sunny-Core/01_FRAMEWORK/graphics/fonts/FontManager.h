#pragma once

#include "../../include.h"
#include "Font.h"

namespace sunny
{
	namespace graphics
	{
		class FontManager
		{
		private:
			static std::vector<Font*> s_fonts;
			static maths::vec2 s_scale;

		public:
			FontManager() = delete;

			static void SetScale(const maths::vec2& scale);
			static inline const maths::vec2& GetScale() { return s_scale; }
		
			static void Add(Font* font);
			
			static void Clean();

			static Font* Get();
			static Font* Get(const std::string& name);
			static Font* Get(unsigned int size);
			static Font* Get(const std::string& name, unsigned int size);
		};
	}
}
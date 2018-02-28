#pragma once

#include "../../sunny.h"

#include "../../maths/vec2.h"
#include "../../maths/Rectangle.h"

#include "../../directx/Texture2D.h"

namespace ftgl
{
	struct texture_atlas_t;
	struct texture_font_t;
}

namespace sunny
{
	namespace graphics
	{
		class Font
		{
		private:
			ftgl::texture_atlas_t* m_FTAtlas;
			ftgl::texture_font_t*  m_FTFont;

			float m_size;
			maths::vec2 m_scale;

			std::string m_name;
			std::string m_fileName;

			mutable directx::Texture2D* m_texture;
		
		public:
			Font(const std::string& name, const std::string& fileName, float size);
			Font(const std::string& name, const unsigned char* data, unsigned int dataSize, float size);

			void SetScale(float x, float y);

			inline void SetScale(const maths::vec2& scale) { m_scale = scale; }
			inline const maths::vec2& GetScale() const { return m_scale; }
		
			inline ftgl::texture_font_t* GetFTFont() const { return m_FTFont; }
			inline const std::string& GetName() const { return m_name; }
			inline const std::string& GetFileName() const { return m_fileName; }
			inline float GetFontSize() const { return m_size; }

			maths::vec2 GetOffsets(const std::string& text) const;
			float GetWidth (const std::string& text) const;
			float GetHeight(const std::string& text) const;
			maths::vec2 GetSize(const std::string& text) const;
			maths::Rectangle GetBounds(const std::string& text) const;

			directx::Texture2D* GetTexture() const;

		private:
			void UpdateAtlas() const;
		};
	}
}
#include "Font.h"

#include "../../dependency/freetype/freetype-gl/freetype-gl.h"

#if _WIN32 || _WIN64
#if _WIN64
#pragma comment(lib, "01_FRAMEWORK/dependency/freetype/win64/freetype.lib")
#else
#pragma comment(lib, "01_FRAMEWORK/dependency/freetype/win32/freetype.lib")
#endif
#endif


namespace sunny
{
	namespace graphics
	{
		using namespace ftgl;

		Font::Font(const std::string& name, const std::string& fileName, float size)
		: m_name(name), m_fileName(fileName), m_size(size), m_scale(maths::vec2(1.0f, 1.0f)), m_texture(nullptr)
		{
			m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
			m_FTFont  = ftgl::texture_font_new_from_file(m_FTAtlas, size, fileName.c_str());
			
			m_texture = new directx::Texture2D(512, 512);
			m_texture->SetData(m_FTAtlas->data);
		}

		Font::Font(const std::string& name, const unsigned char* data, unsigned int dataSize, float size)
		: m_name(name), m_fileName("NULL"), m_size(size), m_scale(maths::vec2(1.0f, 1.0f)), m_texture(nullptr)
		{
			m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
			m_FTFont = ftgl::texture_font_new_from_memory(m_FTAtlas, size, data, dataSize);

			m_texture = new directx::Texture2D(512, 512);
			m_texture->SetData(m_FTAtlas->data);
		}

		void Font::SetScale(float x, float y)
		{
			m_scale = maths::vec2(x, y);
		}

		maths::vec2 Font::GetOffsets(const std::string& text) const
		{
			
			if (text.empty())
				return maths::vec2(0.0f, 0.0f);

			texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[0]);

			float yo = 0.0f;

			const maths::vec2& scale = m_scale;

			for (int i = 0; i < text.size(); ++i)
			{
				texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);

				float height = glyph->height / scale.y;
				float offset = glyph->offset_y / scale.y - height;
				if (offset < yo) yo = offset;
			}

			return maths::vec2(glyph->offset_x / scale.x, yo);
		}

		float Font::GetWidth(const std::string& text) const
		{
			float width = 0.0f;

			const maths::vec2& scale = m_scale;

			for (int i = 0; i < text.size(); ++i)
			{
				texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);
				if (i > 0)
				{
					float kerning = texture_glyph_get_kerning(glyph, text[i - 1]);
					width += kerning / scale.x;
				}
				width += glyph->advance_x / scale.x;
			}

			return width;
		}

		float Font::GetHeight(const std::string& text) const
		{
			float min = 0.0f, max = 0.0f;

			const maths::vec2& scale = m_scale;

			for (int i = 0; i < text.size(); ++i)
			{
				texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);
				float height = glyph->height / scale.y;
				float offset = glyph->offset_y / scale.y - height;
				if (offset < min) min = offset;
				if (height > max) max = height;
			}

			return abs(min) + abs(max);
		}

		maths::vec2 Font::GetSize(const std::string& text) const
		{
			return maths::vec2(GetWidth(text), GetHeight(text));
		}

		maths::Rectangle Font::GetBounds(const std::string& text) const
		{
			return maths::Rectangle(0.0f, 0.0f, GetWidth(text), GetHeight(text));
		}

		directx::Texture2D* Font::GetTexture() const
		{
			UpdateAtlas();

			return m_texture;
		}

		void Font::UpdateAtlas() const
		{
			if (m_FTAtlas->dirty)
			{
				m_texture->SetData(m_FTAtlas->data);
				m_FTAtlas->dirty = false;
			}
		}
	}
}
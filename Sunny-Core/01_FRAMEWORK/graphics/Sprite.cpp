#include "Sprite.h"

namespace sunny
{
	namespace graphics
	{
		Sprite::Sprite() : Renderable2D() {}

		Sprite::Sprite(directx::Texture2D* texture)
		: Renderable2D(maths::vec3(0.0f, 0.0f, 0.0f), maths::vec2((float)texture->GetWidth(), (float)texture->GetHeight()), maths::vec4(1, 1, 1, 1))
		{
			m_texture = texture;
		}

		Sprite::Sprite(float x, float y, directx::Texture2D* texture)
		: Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2((float)texture->GetWidth(), (float)texture->GetHeight()), maths::vec4(1, 1, 1, 1))
		{
			m_texture = texture;
		}

		Sprite::Sprite(float x, float y, float width, float height, const maths::vec4& color, PIVOT pivot)
		: Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), color, pivot)
		{
		}
		
		Sprite::Sprite(float x, float y, float width, float height, directx::Texture2D* texture, PIVOT pivot)
		: Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), maths::vec4(1, 1, 1, 1), pivot)
		{
			m_texture = texture;
		}

		void Sprite::SetUV(const std::vector<maths::vec2>& uv)
		{
			m_UVs = uv;
		}
	}
}
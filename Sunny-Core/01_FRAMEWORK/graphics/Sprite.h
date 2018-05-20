#pragma once

#include "renderables/Renderable2D.h"

namespace sunny
{
	namespace graphics
	{
		class Sprite : public Renderable2D
		{
		protected:
			Sprite();

		public:
			Sprite(directx::Texture2D* texture);
			Sprite(float x, float y, directx::Texture2D* texture);
			Sprite(float x, float y, float width, float height, const maths::vec4& color, PIVOT pivot = PIVOT_LEFT);
			Sprite(float x, float y, float width, float height, directx::Texture2D* texture, PIVOT pivot = PIVOT_LEFT);

			void SetUV(const std::vector<maths::vec2>& uv);

			inline void SetTexture(directx::Texture2D* texture) { m_texture = texture; }
		};
	}
}
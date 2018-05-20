#pragma once

#include "../../include.h"

#include "../../maths/maths.h"

#include "../../directx/Texture2D.h";
#include "../../directx/VertexBuffer.h"
#include "../../directx/IndexBuffer.h"
#include "../../directx/Shader.h"

namespace sunny
{
	namespace graphics
	{
		class Renderer2D;

		struct Vertex2DData
		{
			maths::vec3 vertex; 
			maths::vec2 uv;
			maths::vec2 mask_uv;
			float tid;
			float mid;
			maths::vec4 color;
		};

#define RENDERER_VERTEX_SIZE sizeof(Vertex2DData)

		// 스프라이트와 라벨의 부모

		// 포지션은 왼쪽 하단 or 중앙 앵커포인트를 가진다.
		class Renderable2D
		{		
		protected:
			maths::Rectangle m_bounds;
			maths::vec4 m_color;
			std::vector<maths::vec2> m_UVs;
			directx::Texture* m_texture;
			bool m_visible;

		protected:
			Renderable2D();

		public:
			Renderable2D(const maths::vec2& position, const maths::vec2& size, const maths::vec4& color, PIVOT pivot = PIVOT_LEFT);
			virtual ~Renderable2D();

			virtual void Submit(Renderer2D* renderer) const;

			inline const maths::vec2& GetPosition() const { return m_bounds.position; }
			inline const maths::vec2& GetSize()     const { return m_bounds.size;     }

			inline const maths::Rectangle& GetBounds() const { return m_bounds; }
			inline maths::Rectangle& GetBounds() { return m_bounds; }

			inline maths::AABB GetBoundingBox() const { return m_bounds; }
			inline const maths::vec4& GetColor() const { return m_color; }

			inline directx::Texture* GetTexture() const { return m_texture; }
			inline const std::vector<maths::vec2>& GetUVs() const { return m_UVs; }

			inline bool IsVisible() const { return m_visible; }

			inline void SetPosition(const maths::vec2& position) { m_bounds.position = position; }
			inline void SetSize(const maths::vec2& size) { m_bounds.size = size; }
			inline void SetColor(const maths::vec4& color) { m_color = color; }

			inline void SetVisible(bool visible) { m_visible = visible; }

		public:
			static const std::vector<maths::vec2>& GetDefaultUVs();
		};
	}
}
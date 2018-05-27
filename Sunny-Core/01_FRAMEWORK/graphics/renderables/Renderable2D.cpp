#include "Renderable2D.h"

#include "../renderers/Renderer2D.h"

namespace sunny
{
	namespace graphics
	{
		Renderable2D::Renderable2D() : m_texture(nullptr), m_visible(true)
		{
			m_UVs = GetDefaultUVs();
		}

		Renderable2D::Renderable2D(const maths::vec2& position, const maths::vec2& size, const maths::vec4& color, PIVOT pivot)
		: m_bounds(position, size, pivot), m_color(color), m_texture(nullptr), m_visible(true)
		{
			m_UVs = GetDefaultUVs();
		}

		Renderable2D::~Renderable2D()
		{
		}

		void Renderable2D::Submit(Renderer2D* renderer) const
		{
			renderer->Submit(this);
		}

		const std::vector<maths::vec2>& Renderable2D::GetDefaultUVs()
		{
			// 시계 방향
			static std::vector<maths::vec2> results;
			
			if (!results.size())
			{
				results.push_back(maths::vec2(0, 1));
				results.push_back(maths::vec2(1, 1));
				results.push_back(maths::vec2(1, 0));
				results.push_back(maths::vec2(0, 0));
			}

			return results;
		}
	}
}
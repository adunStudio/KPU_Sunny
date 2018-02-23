#include "Renderable3D.h"

namespace sunny
{
	namespace graphics
	{
		Renderable3D::Renderable3D()
		: m_transform(maths::mat4::Identity()), m_texture(nullptr), m_visible(true), m_color(maths::vec4(1, 1, 1, 1))
		{

		}

		Renderable3D::Renderable3D(const maths::mat4& transform)
		: m_transform(transform), m_texture(nullptr), m_visible(true), m_color(maths::vec4(1, 1, 1, 1))
		{

		}
	}
}

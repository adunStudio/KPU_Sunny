#include "Renderable3D.h"

namespace sunny
{
	namespace graphics
	{
		using namespace maths;

		unsigned int Renderable3D::s_id = 0;

		/*Renderable3D::Renderable3D()
		: m_transform(maths::mat4::Identity()), m_texture(nullptr), m_visible(true), m_color(maths::vec4(1, 1, 1, 1))
		{
		}*/

		Renderable3D::Renderable3D(const maths::mat4& transform)
		:  m_mesh(nullptr), m_materialInstance(nullptr), m_visible(true), m_color(maths::vec4(1, 1, 1, 1))
		{
			m_id = ++s_id;

			AddComponent(new component::TransformComponent(transform));
		}

		void Renderable3D::AddComponent(component::Component* component)
		{
			if (!component->GetType())
			{
				// Debug System
				std::cout << "Renderable3D::AddComponent()" << std::endl;
				exit(1);
			}

			m_components[component->GetType()] = component;
		}

		const maths::vec4& Renderable3D::GetIDColor()
		{
			float r = ((m_id & 0x000000FF) >> 0) / 255.0f;
			float g = ((m_id & 0x0000FF00) >> 8) / 255.0f;
			float b = ((m_id & 0x00FF0000) >> 16)/ 255.0f;
		
			return vec4(r, g, b, 1);
		}
	}
}

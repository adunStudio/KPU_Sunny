#include "Renderable3D.h"

namespace sunny
{
	namespace graphics
	{
		using namespace maths;

		/*Renderable3D::Renderable3D()
		: m_transform(maths::mat4::Identity()), m_texture(nullptr), m_visible(true), m_color(maths::vec4(1, 1, 1, 1))
		{
		}*/

		Renderable3D::Renderable3D(const maths::mat4& transform)
		: m_texture(nullptr), m_visible(true), m_color(maths::vec4(1, 1, 1, 1))
		{
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

	}
}

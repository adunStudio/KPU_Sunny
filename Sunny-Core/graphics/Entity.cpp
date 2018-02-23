#include "Entity.h"

namespace sunny
{
	namespace graphics
	{
		Entity::Entity() : Renderable3D() {}

		Entity::Entity(Mesh* mesh, directx::Texture2D* texture, const mat4& transform)
		: Renderable3D(transform)
		{
			m_mesh = mesh;
			m_texture = texture;
		}

		Entity::Entity(Mesh* mesh, const maths::vec4& color, const mat4& transform)
		: Renderable3D(transform)
		{
			m_mesh = mesh;
			m_color = color;
		}

		void Entity::Render()
		{
			if (m_texture)
				m_texture->Bind();

			m_mesh->Render();

			if (m_texture)
				m_texture->UnBind();
		}
	}
}
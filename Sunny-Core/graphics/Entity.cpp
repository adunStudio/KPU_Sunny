#include "Entity.h"

namespace sunny
{
	namespace graphics
	{
		Entity::Entity() : Renderable3D() {}

		Entity::Entity(Mesh* mesh, directx::Texture2D* texture, const mat4& transform)
		: Renderable3D(transform), m_frame(0), m_materialInstance(nullptr)
		{
			m_mesh = mesh;
			m_texture = texture;
		}

		Entity::Entity(Mesh* mesh, const maths::vec4& color, const mat4& transform)
		: Renderable3D(transform), m_frame(0), m_materialInstance(nullptr)
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

		void Entity::PlayAnimation()
		{
			if (m_mesh->GetType() == MESH_TYPE::STATIC)
				return;

			m_frame++;

			if (m_frame > m_mesh->GetAnimationLength()-1) m_frame = 0;

			m_mesh->SetFrame(m_frame);
		}

	}
}
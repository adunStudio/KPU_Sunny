#include "Entity.h"

namespace sunny
{
	namespace graphics
	{
		Entity::Entity(Mesh* mesh, directx::Texture* texture, const mat4& transform)
		: Renderable3D(transform), m_frame(0)
		{
			m_mesh = mesh;
			m_textures.push_back(texture);
		}

		Entity::Entity(Mesh* mesh, directx::Texture* texture1, directx::Texture* texture2, const mat4& transform)
		: Renderable3D(transform), m_frame(0)
		{
			m_mesh = mesh;
			m_textures.push_back(texture1);
			m_textures.push_back(texture2);
		}

		Entity::Entity(Mesh* mesh, const maths::vec4& color, const mat4& transform)
		: Renderable3D(transform), m_frame(0)
		{
			m_mesh = mesh;
			m_color = color;
		}

		void Entity::Render()
		{
			if (m_materialInstance)
				m_materialInstance->Bind();

			for (int i = 0; i < m_textures.size(); ++i)
				m_textures[i]->Bind(i);

			m_mesh->Render();

			for (int i = 0; i < m_textures.size(); ++i)
				m_textures[i]->UnBind(i);

			if (m_materialInstance)
				m_materialInstance->UnBind();
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
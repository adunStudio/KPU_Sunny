#include "Terrain.h"

namespace sunny
{
	namespace graphics
	{
		Terrain::Terrain(Mesh* mesh, directx::Texture2D* texture, HeightMap* heightMap, const mat4& transform)
		: Renderable3D(transform), m_heightMap(heightMap)
		{
			m_mesh = mesh;
			m_texture = texture;
		}

		void Terrain::Render()
		{
			if (m_materialInstance)
				m_materialInstance->Bind();

			if (m_texture)
				m_texture->Bind();

			m_mesh->Render();

			if (m_texture)
				m_texture->UnBind();

			if (m_materialInstance)
				m_materialInstance->UnBind();
		}
	}
}
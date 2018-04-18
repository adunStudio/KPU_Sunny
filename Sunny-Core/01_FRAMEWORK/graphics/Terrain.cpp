#include "Terrain.h"

namespace sunny
{
	namespace graphics
	{
		Terrain::Terrain(int width, int height, Mesh* mesh, directx::Texture2D* texture, HeightMap* heightMap, const mat4& transform)
		: Renderable3D(transform), m_heightMap(heightMap), m_rectangle(Rectangle(0, 0, width / 2, height / 2))
		{
			m_mesh = mesh;
			m_textures.push_back(texture);
		}

		void Terrain::Render()
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

		void Terrain::AdjustPosition(vec3& position)
		{
			static vec2& min = m_rectangle.GetMinimumBound();
			static vec2& max = m_rectangle.GetMaximumBound();

			if (position.x <= min.x) position.x = min.x + 1; 
			if (position.x >= max.x) position.x = max.x - 1;
			if (position.z <= min.y) position.z = min.y + 1;
			if (position.z >= max.y) position.z = max.y - 1;
		}
	}
}
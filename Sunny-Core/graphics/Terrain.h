#pragma once

#include "../maths/maths.h"
#include "materials/MaterialInstance.h"
#include "renderables/Renderable3D.h"
#include "Model.h"
#include "meshs/Mesh.h"
#include "HeightMap.h"

namespace sunny
{
	namespace graphics
	{
		using namespace maths;
		using namespace component;

		class Terrain : public Renderable3D
		{
		private:
			HeightMap* m_heightMap;

		public:
			Terrain(Mesh* mesh, directx::Texture2D* texture, HeightMap* heightMap, const mat4& transform = mat4::Identity());

			virtual void Render() override;

			inline HeightMap* GetHeightMap() const { return m_heightMap; }
		};
	}
}
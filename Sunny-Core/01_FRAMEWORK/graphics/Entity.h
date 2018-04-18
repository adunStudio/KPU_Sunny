#pragma once

#include "../include.h"

#include "../maths/maths.h"
#include "renderables/Renderable3D.h"
#include "Model.h"
#include "meshs/Mesh.h"
#include "meshs/AnimationMesh.h"

namespace sunny
{
	namespace graphics
	{
		using namespace maths;
		using namespace component;

		class Entity : public Renderable3D
		{
		protected:
			unsigned int m_frame;
			
		public:
			Entity(Mesh* mesh, directx::Texture* texture, const mat4& transform = mat4::Identity());
			Entity(Mesh* mesh, directx::Texture* texture1, directx::Texture* texture2, const mat4& transform = mat4::Identity());
			Entity(Mesh* mesh, const maths::vec4& color = maths::vec4(1, 1, 1, 1), const mat4& transform = mat4::Identity());
						
			virtual void Render() override;

			void PlayAnimation();
		};
	}
}
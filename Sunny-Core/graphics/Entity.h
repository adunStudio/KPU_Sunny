#pragma once

#include "../sunny.h"

#include "../maths/maths.h"
#include "materials/MaterialInstance.h"
#include "renderables/Renderable3D.h"
#include "Model.h"
#include "meshs/Mesh.h"
#include "meshs/AnimationMesh.h"

namespace sunny
{
	namespace graphics
	{
		using namespace maths;

		class Entity : public Renderable3D
		{
		protected:
			Entity();

			unsigned int m_frame;
			
			MaterialInstance* m_materialInstance;

		public:
			Entity(Mesh* mesh, directx::Texture2D* texture, const mat4& transform = mat4::Identity());
			Entity(Mesh* mesh, const maths::vec4& color = maths::vec4(1, 1, 1, 1), const mat4& transform = mat4::Identity());

			inline Mesh* GetMesh() const { return m_mesh; }
			inline const mat4& GetTransform() const { return m_transform; }


			inline void SetMaterial(MaterialInstance* materialInstance) { m_materialInstance = materialInstance; }
			inline MaterialInstance* GetMaterialInstance() const { return m_materialInstance; }


			inline directx::Shader* GetShader() const { return m_materialInstance ? m_materialInstance->GetMaterial()->GetShader() : nullptr; };
			
			virtual void Render() override;

			void PlayAnimation();
		};
	}
}
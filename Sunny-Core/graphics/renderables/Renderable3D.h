#pragma once

#include "../../maths/maths.h"

#include "../Mesh.h"
#include "../MeshFactory.h"
#include "../../directx/Texture2D.h";
#include "../../directx/VertexBuffer.h"
#include "../../directx/IndexBuffer.h"
#include "../../directx/Shader.h"

namespace sunny
{
	namespace graphics
	{
		class Renderable3D
		{
		public:
			enum class RenderFlags
			{
				DISABLE_DEPTH_TEST = BIT(0),
				WIREFRAME = BIT(1)
			};

		protected:
			Mesh* m_mesh;
			maths::mat4 m_transform;
			directx::Texture* m_texture;
			maths::vec4 m_color;

			int  m_renderFlags;
			bool m_visible;

		protected:
			Renderable3D();

		public:

			Renderable3D(const maths::mat4& transform);

			inline const maths::mat4& GetTransform() const { return m_transform; }
			inline const maths::vec4& GetColor()     const { return m_color;     }
			inline const float& GetHasTexture() const { return m_texture ? 1.0f : 0.0f; }

			virtual void Render() = 0;
		};
	}
}
#pragma once

#include "../../maths/maths.h"

#include "../meshs/Mesh.h"
#include "../meshs/MeshFactory.h"
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

			maths::vec3 m_position;
			maths::mat4 m_rotation;
			maths::vec3 m_scale;

		protected:
			Renderable3D();

			void SetTransform();

		public:

			Renderable3D(const maths::mat4& transform);

			inline const maths::mat4& GetTransform() const { return m_transform; }
			inline const maths::vec4& GetColor()     const { return m_color;     }
			inline const float& GetHasTexture() const { return m_texture ? 1.0f : 0.0f; }

			void Translate(const maths::vec3& translation);
			void Rotate(float angle, const maths::vec3& axis);
			void Scale(const maths::vec3& scale);

			inline const maths::vec3& GetPosition() const { return m_position; }
			inline const maths::mat4& GetRotation() const { return m_rotation; }
			inline const maths::vec3& GetScale()    const { return m_scale;    }

			virtual void Render() = 0;
		};
	}
}
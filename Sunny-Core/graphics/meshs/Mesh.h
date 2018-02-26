#pragma once

#include "../../sunny.h"

#include "../../directx/VertexArray.h"
#include "../../directx/IndexBuffer.h"

namespace sunny
{
	namespace graphics
	{
		enum MESH_TYPE
		{
			STATIC    = BIT(0),
			ANIMATION = BIT(1)
		};

		struct Vertex3D
		{
			maths::vec3 position;
			maths::vec3 normal;
			maths::vec2 uv;
			maths::vec3 binormal;
			maths::vec3 tangent;
		};

		class Mesh
		{
		protected:
			directx::VertexArray* m_vertexArray;
			directx::IndexBuffer* m_indexBuffer;

		public:
			Mesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer);
			Mesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer, unsigned char animationLength);

			virtual ~Mesh();

			virtual void Render();

			virtual void SetFrame(unsigned int frame) {};

			virtual unsigned int GetAnimationLength() const { return 0; }

			virtual inline MESH_TYPE GetType() const { return MESH_TYPE::STATIC; };
		};
	}
}
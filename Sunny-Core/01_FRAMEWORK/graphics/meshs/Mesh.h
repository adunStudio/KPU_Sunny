#pragma once

#include "../../include.h"

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
			float       tid;
		};

		class Mesh
		{
		protected:
			directx::VertexArray* m_vertexArray;
			directx::IndexBuffer* m_indexBuffer;

		public:
			maths::vec3 center;

		public:
			Mesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer);

			virtual ~Mesh();

			virtual void Render();

			virtual void SetFrame(unsigned int frame) {};

			virtual inline unsigned int GetAnimationLength() const { return 0; }

			virtual inline unsigned int GetCurrentFrame() const { return 0; };

			virtual inline MESH_TYPE GetType() const { return MESH_TYPE::STATIC; };
		};
	}
}
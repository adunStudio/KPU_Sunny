#pragma once

#include "../sunny.h"

#include "../directx/VertexArray.h"
#include "../directx/IndexBuffer.h"

namespace sunny
{
	namespace graphics
	{
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
		private:
			directx::VertexArray* m_vertexArray;
			directx::IndexBuffer* m_indexBuffer;

			unsigned char         m_animationLength;

			unsigned int        m_frame = 0;

		public:
			Mesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer);
			Mesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer, unsigned char animationLength);

			~Mesh();

			void Render();
		};
	}
}
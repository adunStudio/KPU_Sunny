#include "MeshFactory.h"

#include "../shaders/ShaderFactory.h"

namespace sunny
{
	namespace graphics
	{
		namespace MeshFactory
		{
			Mesh* CreateCube(float size)
			{
				Vertex3D data[8];

				memset(data, 0, sizeof(Vertex3D) * 8);

				data[0].position = maths::vec3(-size / 2.0f, -size / 2.0f,  size / 2.0f);
				data[1].position = maths::vec3( size / 2.0f, -size / 2.0f,  size / 2.0f);
				data[2].position = maths::vec3( size / 2.0f,  size / 2.0f,  size / 2.0f);
				data[3].position = maths::vec3(-size / 2.0f,  size / 2.0f,  size / 2.0f);
				data[4].position = maths::vec3(-size / 2.0f, -size / 2.0f, -size / 2.0f);
				data[5].position = maths::vec3( size / 2.0f, -size / 2.0f, -size / 2.0f);
				data[6].position = maths::vec3( size / 2.0f,  size / 2.0f, -size / 2.0f);
				data[7].position = maths::vec3(-size / 2.0f,  size / 2.0f, -size / 2.0f);
			
				data[0].normal = maths::vec3(-1.0f, -1.0f,  1.0f);
				data[1].normal = maths::vec3( 1.0f, -1.0f,  1.0f);
				data[2].normal = maths::vec3( 1.0f,  1.0f,  1.0f);
				data[3].normal = maths::vec3(-1.0f,  1.0f,  1.0f);
				data[4].normal = maths::vec3(-1.0f, -1.0f, -1.0f);
				data[5].normal = maths::vec3( 1.0f, -1.0f, -1.0f);
				data[6].normal = maths::vec3( 1.0f,  1.0f, -1.0f);
				data[7].normal = maths::vec3(-1.0f,  1.0f, -1.0f);

				ShaderFactory::Default3DShader()->Bind();

				directx::VertexBuffer* buffer = new directx::VertexBuffer(directx::BufferUsage::DYNAMIC);
				buffer->SetData(sizeof(Vertex3D) * 8, data);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("position");
				layout.Push<maths::vec3>("normal");
				layout.Push<maths::vec2>("uv");
				layout.Push<maths::vec3>("BINORMAL");
				layout.Push<maths::vec3>("TANGENT");
				buffer->SetLayout(layout);

				directx::VertexArray* va = new directx::VertexArray();
				va->PushBuffer(buffer);

				unsigned int* indices = new unsigned int[36]
				{
					0, 1, 2, 2, 3, 0,
					3, 2, 6, 6, 7, 3,
					7, 6, 5, 5, 4, 7,
					4, 0, 3, 3, 7, 4,
					0, 1, 5, 5, 4, 0,
					1, 5, 6, 6, 2, 1
				};

				directx::IndexBuffer* ib = new directx::IndexBuffer(indices, 36);

				return new Mesh(va, ib);
			}
		}
	}
}
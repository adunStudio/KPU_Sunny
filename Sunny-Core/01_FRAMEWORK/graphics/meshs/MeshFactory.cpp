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
			

				ShaderFactory::Default3DForwardShader()->Bind();

				directx::VertexBuffer* buffer = new directx::VertexBuffer();
				buffer->SetData(sizeof(Vertex3D) * 8, data);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("POSITION");
				layout.Push<maths::vec3>("NORMAL");
				layout.Push<maths::vec2>("TEXCOORD");
				layout.Push<float      >("TID");
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

			Mesh* CreateQuad(float x, float y, float width, float height)
			{
				Vertex3D data[4];

				data[0].position = maths::vec3(x, y, 0);
				data[0].normal = maths::vec3(0, 1, 0);
				data[0].uv = maths::vec2(0, 1);

				data[1].position = maths::vec3(x + width, y, 0);
				data[1].normal = maths::vec3(0, 1, 0);
				data[1].uv = maths::vec2(1, 1);

				data[2].position = maths::vec3(x + width, y + height, 0);
				data[2].normal = maths::vec3(0, 1, 0);
				data[2].uv = maths::vec2(1, 0);

				data[3].position = maths::vec3(x, y + height, 0);
				data[3].normal = maths::vec3(0, 1, 0);
				data[3].uv = maths::vec2(0, 0);

				directx::VertexBuffer* buffer = new directx::VertexBuffer();
				buffer->SetData(sizeof(Vertex3D) * 4, data);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("POSITION");
				layout.Push<maths::vec3>("NORMAL");
				layout.Push<maths::vec2>("TEXCOORD");
				layout.Push<float      >("TID");
				buffer->SetLayout(layout);

				directx::VertexArray* va = new directx::VertexArray();
				va->PushBuffer(buffer);

				unsigned int* indices = new unsigned int[6]{ 0, 1, 2, 2, 3, 0 };

				directx::IndexBuffer* ib = new directx::IndexBuffer(indices, 6);
				
				return new Mesh(va, ib);
			}

			Mesh* CreatePlane(float width, float height, const maths::vec3& normal)
			{
				maths::vec3 vec = normal * 90.0f;
				maths::mat4 rotation = maths::mat4::Identity();//maths::mat4::Rotate(vec.z, maths::vec3(1, 0, 0)) * maths::mat4::Rotate(vec.y, maths::vec3(0, 1, 0)) * maths::mat4::Rotate(vec.x, maths::vec3(0, 0, 1));

				Vertex3D data[4];
				memset(data, 0, sizeof(Vertex3D) * 4);

				data[0].position = rotation * maths::vec3(-width / 2.0f, 0.0f, height / 2.0f);
				data[0].normal   = normal;
				data[0].uv       = maths::vec2(0, 1);
			
				data[1].position = rotation * maths::vec3( width / 2.0f, 0.0f, height / 2.0f);
				data[1].normal = normal;
				data[1].uv = maths::vec2(1, 1);

				data[2].position = rotation * maths::vec3(width / 2.0f, 0.0f, -height / 2.0f);
				data[2].normal = normal;
				data[2].uv = maths::vec2(1, 0);

				data[3].position = rotation * maths::vec3(-width / 2.0f, 0.0f, -height / 2.0f);
				data[3].normal = normal;
				data[3].uv = maths::vec2(0, 0);


				ShaderFactory::Default3DForwardShader()->Bind();

				directx::VertexBuffer* buffer = new directx::VertexBuffer();
				buffer->SetData(sizeof(Vertex3D) * 8, data);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("POSITION");
				layout.Push<maths::vec3>("NORMAL");
				layout.Push<maths::vec2>("TEXCOORD");
				layout.Push<float      >("TID");
				buffer->SetLayout(layout);

				directx::VertexArray* va = new directx::VertexArray();
				va->PushBuffer(buffer);

				unsigned int* indices = new unsigned int[6]
				{
					0, 1, 2,
					2, 3, 0
				};

				directx::IndexBuffer* ib = new directx::IndexBuffer(indices, 6);

				return new Mesh(va, ib);
			}

			Mesh* CreateXAxis()
			{
				Vertex3D data[4];

				data[0].position = maths::vec3(0, 0, 0);
				data[0].normal = maths::vec3(0, 1, 1);
				data[0].uv = maths::vec2(0, 1);

				data[1].position = maths::vec3(15, 0, 0);
				data[1].normal = maths::vec3(0, 1, 1);
				data[1].uv = maths::vec2(1, 1);

				data[2].position = maths::vec3(15, 0.1, 0);
				data[2].normal = maths::vec3(0, 1, 1);
				data[2].uv = maths::vec2(1, 0);

				data[3].position = maths::vec3(0, 0.1, 0);
				data[3].normal = maths::vec3(0, 1, 1);
				data[3].uv = maths::vec2(0, 0);

				ShaderFactory::Default3DForwardShader()->Bind();

				directx::VertexBuffer* buffer = new directx::VertexBuffer();
				buffer->SetData(sizeof(Vertex3D) * 4, data);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("POSITION");
				layout.Push<maths::vec3>("NORMAL");
				layout.Push<maths::vec2>("TEXCOORD");
				layout.Push<float      >("TID");
				buffer->SetLayout(layout);

				directx::VertexArray* va = new directx::VertexArray();
				va->PushBuffer(buffer);

				unsigned int* indices = new unsigned int[6]{ 0, 1, 2, 2, 3, 0 };

				directx::IndexBuffer* ib = new directx::IndexBuffer(indices, 6);

				return new Mesh(va, ib);
			}

			Mesh* CreateYAxis()
			{
				Vertex3D data[4];

				data[0].position = maths::vec3(0, 0, 0);
				data[0].normal = maths::vec3(0, 1, 1);
				data[0].uv = maths::vec2(0, 1);

				data[1].position = maths::vec3(0.1, 0, 0);
				data[1].normal = maths::vec3(0, 1, 1);
				data[1].uv = maths::vec2(1, 1);

				data[2].position = maths::vec3(0.1, 15, 0);
				data[2].normal = maths::vec3(0, 1, 1);
				data[2].uv = maths::vec2(1, 0);

				data[3].position = maths::vec3(0, 15, 0);
				data[3].normal = maths::vec3(0, 1, 1);
				data[3].uv = maths::vec2(0, 0);

				ShaderFactory::Default3DForwardShader()->Bind();

				directx::VertexBuffer* buffer = new directx::VertexBuffer();
				buffer->SetData(sizeof(Vertex3D) * 4, data);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("POSITION");
				layout.Push<maths::vec3>("NORMAL");
				layout.Push<maths::vec2>("TEXCOORD");
				layout.Push<float      >("TID");
				buffer->SetLayout(layout);

				directx::VertexArray* va = new directx::VertexArray();
				va->PushBuffer(buffer);

				unsigned int* indices = new unsigned int[6]{ 0, 1, 2, 2, 3, 0 };

				directx::IndexBuffer* ib = new directx::IndexBuffer(indices, 6);

				return new Mesh(va, ib);
			}

			Mesh* CreateZAxis()
			{
				Vertex3D data[4];

				data[0].position = maths::vec3(0, 0, 0);
				data[0].normal = maths::vec3(0, 1, 1);
				data[0].uv = maths::vec2(0, 1);

				data[1].position = maths::vec3(0.1, 0, 0);
				data[1].normal = maths::vec3(0, 1, 1);
				data[1].uv = maths::vec2(1, 1);

				data[2].position = maths::vec3(0.1, 0, 15);
				data[2].normal = maths::vec3(0, 1, 1);
				data[2].uv = maths::vec2(1, 0);

				data[3].position = maths::vec3(0.1, 0, 15);
				data[3].normal = maths::vec3(0, 1, 1);
				data[3].uv = maths::vec2(0, 0);

				ShaderFactory::Default3DForwardShader()->Bind();

				directx::VertexBuffer* buffer = new directx::VertexBuffer();
				buffer->SetData(sizeof(Vertex3D) * 4, data);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("POSITION");
				layout.Push<maths::vec3>("NORMAL");
				layout.Push<maths::vec2>("TEXCOORD");
				layout.Push<float      >("TID");
				buffer->SetLayout(layout);

				directx::VertexArray* va = new directx::VertexArray();
				va->PushBuffer(buffer);

				unsigned int* indices = new unsigned int[6]{ 0, 1, 2, 2, 3, 0 };

				directx::IndexBuffer* ib = new directx::IndexBuffer(indices, 6);

				return new Mesh(va, ib);
			}

			
			Mesh* CreateGeometry(float width, float depth, int m, int n)
			{
				int vertexCount = m * n;
				int indicesCount = (m - 1) * (n - 1) * 2;

				Vertex3D* data = new Vertex3D[vertexCount];

				float halfWidth = 0.5f * width;
				float halfDepth = 0.5f * depth;

				float dx = width / (n - 1);
				float dz = depth / (m - 1);

				float du = 1.0f / (n - 1);
				float dv = 1.0f / (m - 1);

				for (int i = 0; i < m; ++i)
				{
					float z = halfDepth - i * dz;
					//float z = i * dz;

					for (int j = 0; j < n; ++j)
					{
						float x = -halfWidth + j * dx;
						//float x = j * dx;

						float y = 0;// 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));

						data[i * n + j].position = maths::vec3(x, y, z);
						data[i * n + j].normal = maths::vec3(0, 1, 0);
						data[i * n + j].uv = maths::vec2(j * du, i * dv);
						data[i * n + j].tid = 0;
					}
				}

				ShaderFactory::Default3DForwardShader()->Bind();

				directx::VertexBuffer* buffer = new directx::VertexBuffer();
				buffer->SetData(sizeof(Vertex3D) * vertexCount, data);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("POSITION");
				layout.Push<maths::vec3>("NORMAL");
				layout.Push<maths::vec2>("TEXCOORD");
				layout.Push<float      >("TID");
				buffer->SetLayout(layout);

				directx::VertexArray* va = new directx::VertexArray();
				va->PushBuffer(buffer);

				unsigned int* indices = new unsigned int[indicesCount * 3];// { 0, 1, 2, 2, 3, 0 };

				int k = 0;

				for (int i = 0; i < m - 1; ++i)
				{
					for (int j = 0; j < n - 1; ++j)
					{
						indices[k]     = i * n + j;
						indices[k + 1] = i * n + j + 1;
						indices[k + 2] = (i + 1) * n + j;
						indices[k + 3] = (i + 1) * n + j;
						indices[k + 4] = i * n + j + 1;
						indices[k + 5] = (i + 1) * n + j + 1;

						k += 6;
					}
				}

				directx::IndexBuffer* ib = new directx::IndexBuffer(indices, indicesCount * 3);

				return new Mesh(va, ib);
			}

		}
	}
}
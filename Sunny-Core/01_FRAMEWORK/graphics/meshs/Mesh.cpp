#include "Mesh.h"

#include "../renderers/Renderer3D.h"

namespace sunny
{
	namespace graphics
	{
		Mesh::Mesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer)
		: m_vertexArray(vertexArray), m_indexBuffer(indexBuffer)
		{
		}
	
		Mesh::~Mesh()
		{
			delete m_vertexArray;
			delete m_indexBuffer;
		}

		void Mesh::Render()
		{
			m_indexBuffer->Bind();
			m_vertexArray->Draw(m_indexBuffer->GetCount());
		}
	}
}
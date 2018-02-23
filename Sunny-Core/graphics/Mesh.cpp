#include "Mesh.h"

#include "renderers/Renderer3D.h"

namespace sunny
{
	namespace graphics
	{
		Mesh::Mesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer)
		: m_vertexArray(vertexArray), m_indexBuffer(indexBuffer), m_animationLength(0)
		{
		}
	
		Mesh::Mesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer, unsigned char animationLength)
		: m_vertexArray(vertexArray), m_indexBuffer(indexBuffer), m_animationLength(animationLength)
		{
		}

		Mesh::~Mesh()
		{
			delete m_vertexArray;
			delete m_indexBuffer;
		}

		void Mesh::Render()
		{
			
			if (m_animationLength > 0)
			{
				m_frame++;

				if (m_frame == m_animationLength)
					m_frame = 0;

				m_indexBuffer->Bind();
				m_vertexArray->DrawOne(m_indexBuffer->GetCount(), m_frame);
			}
			else
			{
				m_indexBuffer->Bind();
				m_vertexArray->Draw(m_indexBuffer->GetCount());
			}
		}
	}
}
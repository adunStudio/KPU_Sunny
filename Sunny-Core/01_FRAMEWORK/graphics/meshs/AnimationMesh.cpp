#include "AnimationMesh.h"

namespace sunny
{
	namespace graphics
	{
		AnimationMesh::AnimationMesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer, unsigned int animationLength)
		: Mesh(vertexArray, indexBuffer), m_animationLength(animationLength), m_currentFrame(0)
		{
		}

		void AnimationMesh::Render()
		{
			m_indexBuffer->Bind();
			m_vertexArray->DrawOne(m_indexBuffer->GetCount(), m_currentFrame);
		}

		void AnimationMesh::SetFrame(unsigned int frame)
		{
			if (frame > m_animationLength) frame = 0;

			m_currentFrame = frame;
		}

	}
}
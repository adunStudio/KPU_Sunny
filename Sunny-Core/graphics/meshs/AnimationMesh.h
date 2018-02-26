#pragma once

#include "../../sunny.h"

#include "Mesh.h"

namespace sunny
{
	namespace graphics
	{
		class AnimationMesh : public Mesh
		{
		private:
			unsigned int m_animationLength;
			unsigned int m_currentFrame;

		public:
			AnimationMesh(directx::VertexArray* vertexArray, directx::IndexBuffer* indexBuffer, unsigned int animationLength);

			~AnimationMesh() override {};

			void Render() override;

			void SetFrame(unsigned int frame);

			unsigned int GetAnimationLength() const { return m_animationLength; }

			inline MESH_TYPE GetType() const { return MESH_TYPE::ANIMATION; };
		};
	}
}
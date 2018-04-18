#pragma once

#include "../../include.h"

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

			void SetFrame(unsigned int frame) override;

			inline unsigned int GetAnimationLength() const override { return m_animationLength; };
			
			inline unsigned int GetCurrentFrame() const override { return m_currentFrame; };

			inline MESH_TYPE GetType() const override { return MESH_TYPE::ANIMATION; };
		};
	}
}
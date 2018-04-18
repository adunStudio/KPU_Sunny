#pragma once

#include "../directx/DeferredBuffer.h"

namespace sunny
{
	namespace graphics
	{
		class GBuffer
		{
		public:
			enum class TextureType : int
			{
				POSITION = 0, NORMAL = 1, DIFFUSE = 2
			};

		private:
			directx::DeferredBuffer* m_buffer;

		private:
			void Init();

		public:
			GBuffer();
			~GBuffer();

			void SetGBufferTexture(TextureType type);
			void SetGBufferSampler();

			void Bind();
			void UnBind();

			void Draw();
		};
	}
}
#pragma once

#include "../directx/GeometryBuffer.h"

namespace sunny
{
	namespace graphics
	{
		using namespace directx;
		
		enum GBufferType
		{
			DEFERRED  = BIT(0),
			SHADOWMAP = BIT(1)
		};

		enum GBufferTextureIndex
		{
			DIFFUSE = 8,
			NORMAL  = 9,
			SHADOW  = 7
		};

		enum GBufferSapmaerIndex
		{
			DEFERRED_SAMPLER  = 8,
			SHADOW_SMAPLER    = 7
		};

		class GBuffer
		{			
		private:
			directx::GeometryBuffer* m_buffer;

		private:
			void Init();

		public:
			GBuffer();
			~GBuffer();

			void SetGBuffer(unsigned int buffer);

			void Bind(GBufferType type);
			void UnBind();

			const unsigned char* GetDiffuseData();

			void Draw();
		};
	}
}
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
			DIFFUSE  = 7,
			NORMAL   = 8,
			POSITION = 9,
			DEPTH    = 10,
			ID       = 11,
			SHADOW   = 12,
		};

		enum GBufferSapmaerIndex
		{
			DEFERRED_SAMPLER  = 1,
			SHADOW_SMAPLER    = 2
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

			const unsigned char* GetIDData();

			void Draw();
		};
	}
}
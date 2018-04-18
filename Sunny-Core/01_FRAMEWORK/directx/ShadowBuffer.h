#pragma once

#include "../include.h"
#include "Common.h"
#include "Context.h"

namespace sunny
{
	namespace directx
	{
		class ShadowBuffer
		{
		private:
			static ShadowBuffer* s_instance;

		private:
			Context* m_context;

			ID3D11DepthStencilView*   m_depthStencilView;
			ID3D11ShaderResourceView* m_shaderResourceView;
			ID3D11SamplerState*       m_sampler;
			D3D11_VIEWPORT            m_viewport;
		
		public:
			static void Init();

			inline static ShadowBuffer* GetShadowBuffer() { return s_instance; }

			inline static void Bind()   { s_instance->BindInternal(); }
			inline static void UnBind() { s_instance->UnBindInternal(); }

			inline static ID3D11DepthStencilView*   GetDepthStencilBuffer() { return s_instance->m_depthStencilView; }
			inline static ID3D11ShaderResourceView* GetShaderResource() { return s_instance->m_shaderResourceView; }
			inline static ID3D11SamplerState*       GetSamplerState() { return s_instance->m_sampler; }

		private:
			void Resize();

			void BindInternal();
			void UnBindInternal();

		public:
			ShadowBuffer();
		};
	}
}
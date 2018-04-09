#pragma once

#include "../sunny.h"
#include "Common.h"
#include "Context.h"

namespace sunny
{
	namespace directx
	{
		class DeferredBuffer
		{
		private:
			static DeferredBuffer* s_instance;

		public:
			const static unsigned int BUFFER_COUNT = 3;

		private:
			Context* m_context;

			ID3D11Texture2D*		  m_textures[BUFFER_COUNT];
			ID3D11RenderTargetView*   m_renderTargetViews[BUFFER_COUNT];
			ID3D11ShaderResourceView* m_shaderResourceViews[4];
			ID3D11Texture2D*          m_depthStencilBuffer;
			ID3D11DepthStencilView*   m_depthStencilView;
			ID3D11DepthStencilView*   m_depthStencilView2;
			ID3D11SamplerState*       m_sampler;

		public:
			static void Init();

		protected:
			void InitInternal();

			void BindInternal();
			void UnBindInternal();

			void Resize();

		public:
			DeferredBuffer();

			inline static void Bind()   { s_instance->BindInternal(); }
			inline static void UnBind() { s_instance->UnBindInternal(); }

			inline static DeferredBuffer* GetDeferredBuffer() { return s_instance; }

			inline static ID3D11RenderTargetView* GetBuffer(int index) { return s_instance->m_renderTargetViews[index]; }

			inline static ID3D11DepthStencilView* GetDepthStencilBuffer() { return s_instance->m_depthStencilView; }

			inline static ID3D11ShaderResourceView* GetShaderResource(int index) { return s_instance->m_shaderResourceViews[index]; };
		
			inline static ID3D11SamplerState* GetSamplerState() { return s_instance->m_sampler; }
		};
	}
}
#pragma once

#include "../sunny.h"
#include "Common.h"
#include "Context.h"

namespace sunny
{
	namespace directx
	{
		class DebugBuffer
		{
		private:
			static DebugBuffer* s_instance;

		public:
		private:
			Context* m_context;

			ID3D11RenderTargetView*   m_renderTargetView;
			ID3D11DepthStencilView*   m_depthStencilView;
			ID3D11SamplerState*       m_sampler;
			D3D11_VIEWPORT            m_viewport;
			D3D11_VIEWPORT            m_viewport2;

		public:
			static void Init();

		protected:
			void InitInternal();

			void BindInternal();
			void BindInternal2();
			void UnBindInternal();

			void Resize();

		public:
			DebugBuffer();

			inline static void Bind() { s_instance->BindInternal(); }
			inline static void Bind2() { s_instance->BindInternal2(); }
			inline static void UnBind() { s_instance->UnBindInternal(); }

			inline static DebugBuffer* GetDebugBuffer() { return s_instance; }

			inline static ID3D11RenderTargetView* GetBuffer() { return s_instance->m_renderTargetView; }

			inline static ID3D11DepthStencilView* GetDepthStencilBuffer() { return s_instance->m_depthStencilView; }

			inline static ID3D11SamplerState* GetSamplerState() { return s_instance->m_sampler; }
		};
	}
}
#pragma once

#include "../include.h"
#include "Common.h"
#include "Context.h"

namespace sunny
{
	namespace directx
	{
		enum GeometryTextureType : int
		{
			DIFFUSE = 0,
			NORMAL  = 1,
			DEPTH   = 2
		};

		class GeometryBuffer
		{
		public:
			const static unsigned int BUFFER_COUNT = 3;

			static void Init();

		private:
			static GeometryBuffer* s_instance;

			GeometryBuffer();

			float m_width;
			float m_height;

			Context* m_context;

			ID3D11RenderTargetView*   m_renderTargetViews[BUFFER_COUNT];
			ID3D11DepthStencilView*   m_depthStencilView;

			ID3D11ShaderResourceView* m_shaderResourceViews[BUFFER_COUNT];
			ID3D11SamplerState*       m_samplers[2];

			D3D11_VIEWPORT            m_viewport;

		protected:
			void InitInternal();

			void InitRenderTarget();
			void InitDepthStencil();

			void BindInternal();
			void UnBindInternal();

			void Resize();

		public:

			inline static void Bind() { s_instance->BindInternal(); }
			inline static void UnBind() { s_instance->UnBindInternal(); }

			inline static GeometryBuffer* GetGeometryBuffer() { return s_instance; }

			inline static ID3D11RenderTargetView* GetRenderTargeBuffer(GeometryTextureType type)
			{	
				if (type == GeometryTextureType::DEPTH) return nullptr;
				
				return s_instance->m_renderTargetViews[type];
			}

			inline static ID3D11DepthStencilView* GetDepthStencilBuffer() { return s_instance->m_depthStencilView; }

			inline static ID3D11ShaderResourceView* GetShaderResource(GeometryTextureType type) { return s_instance->m_shaderResourceViews[type]; };

			inline static ID3D11SamplerState* GetSamplerState(GeometryTextureType type)
			{ 
				if (type == GeometryTextureType::DEPTH) s_instance->m_samplers[1];

				return s_instance->m_samplers[0];
			}
		};
	}
}
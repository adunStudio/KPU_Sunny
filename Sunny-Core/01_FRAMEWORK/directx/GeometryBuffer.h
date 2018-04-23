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
			ID3D11DepthStencilView*   m_depthStencilView[2];

			ID3D11ShaderResourceView* m_shaderResourceViews[BUFFER_COUNT];
			ID3D11SamplerState*       m_samplers[2];

			ID3D11Texture2D*          m_textures[3];

			D3D11_MAPPED_SUBRESOURCE  m_mappedSubresource; 
			ID3D11Texture2D*          m_copyTexture;

			D3D11_VIEWPORT            m_viewport;

		protected:
			void InitInternal();

			void InitRenderTarget();
			void InitDepthStencil();
			void InitCopyTarget();

			void CopyInternal();
			const unsigned char* GetDiffuseDataInternal();

			void BindInternal(GeometryTextureType type);
			void UnBindInternal();

			void Resize();

		public:

			inline static void Bind(GeometryTextureType type) { s_instance->BindInternal(type); }
			inline static void UnBind() { s_instance->UnBindInternal(); }


			inline static int GetWidth()  { return s_instance->m_width; };
			inline static int GetHeight() { return s_instance->m_height;};

			inline static GeometryBuffer* GetGeometryBuffer() { return s_instance; }

			inline static ID3D11RenderTargetView* GetRenderTargeBuffer(GeometryTextureType type)
			{	
				if (type == GeometryTextureType::DEPTH) return nullptr;
				
				return s_instance->m_renderTargetViews[type];
			}

			inline static ID3D11DepthStencilView* GetDepthStencilBuffer(unsigned int idx) { return s_instance->m_depthStencilView[idx]; }

			inline static ID3D11ShaderResourceView* GetShaderResource(GeometryTextureType type) { return s_instance->m_shaderResourceViews[type]; };

			inline static ID3D11SamplerState* GetSamplerState(GeometryTextureType type)
			{ 
				if (type == GeometryTextureType::DEPTH) s_instance->m_samplers[1];

				return s_instance->m_samplers[0];
			}

			inline static int GetMapRowPitch() { return s_instance->m_mappedSubresource.RowPitch; }

			inline static const unsigned char* GetDiffuseData() { return s_instance->GetDiffuseDataInternal(); }
		};
	}
}
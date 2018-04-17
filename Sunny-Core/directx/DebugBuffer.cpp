#include "DebugBuffer.h"

namespace sunny
{
	namespace directx
	{
		DebugBuffer* DebugBuffer::s_instance = nullptr;

		DebugBuffer::DebugBuffer()
		{
			m_context = Context::GetContext();
		}

		void DebugBuffer::Init()
		{
			s_instance = new DebugBuffer();
			s_instance->InitInternal();
		}

		void DebugBuffer::InitInternal()
		{
			D3D11_SAMPLER_DESC samplerDesc = {};
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.MaxAnisotropy = 16;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			directx::Context::GetDevice()->CreateSamplerState(&samplerDesc, &m_sampler);

			Resize();
		}

		void DebugBuffer::Resize()
		{
			int width  = Context::GetWindowProperties().width;
			int height = Context::GetWindowProperties().height;
			
			// µð¹ö±× ºäÆ÷Æ®
			m_viewport.TopLeftX = (float)width  - (float)width  / 2;
			m_viewport.TopLeftY = (float)height - (float)height / 2;
			m_viewport.Width    = (float)width / 2;
			m_viewport.Height   = (float)height/ 2;
			m_viewport.MinDepth = 0.0f;
			m_viewport.MaxDepth = 1.0f;

			m_viewport2.TopLeftX = 0;
			m_viewport2.TopLeftY = 0;
			m_viewport2.Width    = (float)width / 2;
			m_viewport2.Height   = (float)height / 2;
			m_viewport2.MinDepth = 0.0f;
			m_viewport2.MaxDepth = 1.0f;


			// µð¹ö±× ÅØ½ºÃÄ ¼³¸í ±¸Á¶Ã¼
			D3D11_TEXTURE2D_DESC debugTextureDesc;
			ZeroMemory(&debugTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
			{
				debugTextureDesc.Width              = width;
				debugTextureDesc.Height             = height;
				debugTextureDesc.MipLevels          = 1;
				debugTextureDesc.ArraySize          = 1;
				debugTextureDesc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
				debugTextureDesc.SampleDesc.Count   = 1;
				debugTextureDesc.SampleDesc.Quality = 0;
				debugTextureDesc.Usage              = D3D11_USAGE_DEFAULT;
				debugTextureDesc.BindFlags          = D3D11_BIND_RENDER_TARGET;
				debugTextureDesc.CPUAccessFlags     = 0;
				debugTextureDesc.MiscFlags          = 0;
			}

			// µð¹ö±× ÅØ½ºÃÄ »ý¼º
			ID3D11Texture2D* m_debugTexture;
			m_context->GetDevice()->CreateTexture2D(&debugTextureDesc, NULL, &m_debugTexture);

			// µð¹ö±× ·»´õ Å¸°Ù ºä ¼³¸í ±¸Á¶Ã¼
			D3D11_RENDER_TARGET_VIEW_DESC debugRenderTargetViewDesc;
			ZeroMemory(&debugRenderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
			{
				debugRenderTargetViewDesc.Format             = debugTextureDesc.Format;
				debugRenderTargetViewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
				debugRenderTargetViewDesc.Texture2D.MipSlice = 0;
			}

			
			// µð¹ö±× ·»´õ Å¸°Ù ºä »ý¼º
			m_context->GetDevice()->CreateRenderTargetView(m_debugTexture, &debugRenderTargetViewDesc, &m_renderTargetView);
			m_debugTexture->Release();
		
				
			// µð¹ö±× ±íÀÌ / ±¼°î ÅØ½ºÃÄ ¼³¸í ±¸Á¶Ã¼
			D3D11_TEXTURE2D_DESC debugDepthBufferDesc;
			ZeroMemory(&debugDepthBufferDesc, sizeof(debugDepthBufferDesc));
			{
				debugDepthBufferDesc.Width              = width;
				debugDepthBufferDesc.Height             = height;
				debugDepthBufferDesc.MipLevels          = 1;
				debugDepthBufferDesc.ArraySize          = 1;
				debugDepthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
				debugDepthBufferDesc.SampleDesc.Count   = 1;
				debugDepthBufferDesc.SampleDesc.Quality = 0;
				debugDepthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
				debugDepthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
				debugDepthBufferDesc.CPUAccessFlags     = 0;
				debugDepthBufferDesc.MiscFlags          = 0;
			}

			// µð¹ö±× ±íÀÌ / ½ºÅÙ½Ç ÅØ½ºÃ³ »ý¼º
			ID3D11Texture2D* debugDepthTexture;
			m_context->GetDevice()->CreateTexture2D(&debugDepthBufferDesc, 0, &debugDepthTexture);


			// µð¹ö±× ±íÀÌ / ½ºÅÙ½Ç ·»´õ Å¸°Ù ¼³¸í ±¸Á¶Ã¼
			D3D11_DEPTH_STENCIL_VIEW_DESC debugDepthStencilViewDesc;
			ZeroMemory(&debugDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			{
				debugDepthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
				debugDepthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
				debugDepthStencilViewDesc.Texture2D.MipSlice = 0;
			}

			// ±íÀÌ/½ºÅÙ½Ç ·»´õ Å¸°Ù »ý¼º
			m_context->GetDevice()->CreateDepthStencilView(debugDepthTexture, NULL, &m_depthStencilView);
			debugDepthTexture->Release();
		}

		void DebugBuffer::BindInternal()
		{
			m_context->GetDeviceContext()->RSSetViewports(1, &m_viewport);
			ID3D11RenderTargetView* renderTarget = directx::Context::GetBackBuffer();
			m_context->GetDeviceContext()->OMSetRenderTargets(1, &renderTarget, m_depthStencilView);
		}

		void DebugBuffer::BindInternal2()
		{
			m_context->GetDeviceContext()->RSSetViewports(1, &m_viewport2);
			ID3D11RenderTargetView* renderTarget = directx::Context::GetBackBuffer();
			m_context->GetDeviceContext()->OMSetRenderTargets(1, &renderTarget, m_depthStencilView);
		}

		void DebugBuffer::UnBindInternal()
		{
			m_context->Bind();
		}
	}
}
#include "ShadowBuffer.h"

namespace sunny
{
	namespace directx
	{
		ShadowBuffer* ShadowBuffer::s_instance = nullptr;

		ShadowBuffer::ShadowBuffer()
		{
			m_context = Context::GetContext();
		}

		void ShadowBuffer::Init()
		{
			s_instance = new ShadowBuffer();
			
			s_instance->Resize();
		}

		void ShadowBuffer::Resize()
		{
			int width  = 2048; // m_context->GetWindowProperties().width;   // 2048
			int height = 2048; // m_context->GetWindowProperties().height;  // 2048

			// 그림자 텍스쳐 설명 구조체
			D3D11_TEXTURE2D_DESC shadowTextureDesc;
			{
				shadowTextureDesc.Width              = width;
				shadowTextureDesc.Height             = height;
				shadowTextureDesc.MipLevels          = 1;
				shadowTextureDesc.ArraySize          = 1;
				shadowTextureDesc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
				shadowTextureDesc.SampleDesc.Count   = 1;
				shadowTextureDesc.SampleDesc.Quality = 0;
				shadowTextureDesc.Usage              = D3D11_USAGE_DEFAULT;
				shadowTextureDesc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				shadowTextureDesc.CPUAccessFlags     = 0;
				shadowTextureDesc.MiscFlags          = 0;
			}

			// 그림자 텍스쳐 생성
			ID3D11Texture2D* shadowTexture = nullptr;
			auto a = m_context->GetDevice()->CreateTexture2D(&shadowTextureDesc, NULL, &shadowTexture);

			std::cout << "a: " << a << std::endl;

			// 그림자 렌더 타겟 뷰 설명 구조체
			D3D11_RENDER_TARGET_VIEW_DESC shadowRenderTargetViewDesc;
			ZeroMemory(&shadowRenderTargetViewDesc, sizeof(shadowRenderTargetViewDesc));
			{
				shadowRenderTargetViewDesc.Format = shadowTextureDesc.Format;
				shadowRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				shadowRenderTargetViewDesc.Texture2D.MipSlice = 0;
			}

			// 그림자 렌더 타겟 뷰 생성
			auto b = m_context->GetDevice()->CreateRenderTargetView(shadowTexture, &shadowRenderTargetViewDesc, &m_renderTargetView);

			std::cout << "b: " << b << std::endl;

			// 그림자 셰이더 리소스 뷰 설명 구조체
			D3D11_SHADER_RESOURCE_VIEW_DESC shadowShaderResourceViewDesc;
			ZeroMemory(&shadowShaderResourceViewDesc, sizeof(shadowShaderResourceViewDesc));
			{
				shadowShaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				shadowShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				shadowShaderResourceViewDesc.Texture2D.MipLevels = 1;
				shadowShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			}

			// 그림자 셰이더 리소스 뷰 생성
			auto c = m_context->GetDevice()->CreateShaderResourceView(shadowTexture, &shadowShaderResourceViewDesc, &m_shaderResourceView);

			std::cout << "c: " << c << std::endl;

			// 그림자 깊이/스텐실 텍스쳐 설명 구조체
			D3D11_TEXTURE2D_DESC shadowDepthTextureDesc;
			ZeroMemory(&shadowDepthTextureDesc, sizeof(shadowDepthTextureDesc));
			{
				shadowDepthTextureDesc.Width = width;
				shadowDepthTextureDesc.Height = height;
				shadowDepthTextureDesc.MipLevels = 1;
				shadowDepthTextureDesc.ArraySize = 1;
				//shadowDepthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				shadowDepthTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
				shadowDepthTextureDesc.SampleDesc.Count = 1;
				shadowDepthTextureDesc.SampleDesc.Quality = 0;
				shadowDepthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
				shadowDepthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				shadowDepthTextureDesc.CPUAccessFlags = 0;
				shadowDepthTextureDesc.MiscFlags = 0;
			}

			// 그림자 깊이/스텐실 텍스쳐 생성
			ID3D11Texture2D* shadowDepthTexture = nullptr;
			auto d = m_context->GetDevice()->CreateTexture2D(&shadowDepthTextureDesc, NULL, &shadowDepthTexture);
			std::cout << "d: " << d << std::endl;

			// ㄱ

			// 그림자 깊이/스텐실 뷰 설명 구조체
			D3D11_DEPTH_STENCIL_VIEW_DESC shadowDepthStencilViewDesc;
			ZeroMemory(&shadowDepthStencilViewDesc, sizeof(shadowDepthStencilViewDesc));
			{
				shadowDepthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
				shadowDepthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
				shadowDepthStencilViewDesc.Texture2D.MipSlice = 0;
			}
			
			auto e = m_context->GetDevice()->CreateDepthStencilView(shadowDepthTexture, &shadowDepthStencilViewDesc, &m_depthStencilView);
			std::cout << "e: " << e << std::endl;


			// 텍스처에 대한 참조는 뷰에 저장되어 있으므로 참조 해제
			shadowTexture->Release();
			shadowDepthTexture->Release();
		}

		void ShadowBuffer::BindInternal()
		{
			m_context->GetDeviceContext()->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		}

		void ShadowBuffer::UnBindInternal()
		{
			m_context->Bind();
		}
	}
}
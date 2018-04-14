#include "DeferredBuffer.h"

namespace sunny
{
	// https://www.3dgep.com/forward-plus/
	namespace directx
	{
		DeferredBuffer* DeferredBuffer::s_instance = nullptr;

		DeferredBuffer::DeferredBuffer()
		{
			m_context = Context::GetContext();
		}

		void DeferredBuffer::Init()
		{
			s_instance = new DeferredBuffer();
			s_instance->InitInternal();
		}

		void DeferredBuffer::InitInternal()
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

		void DeferredBuffer::Resize()
		{
			int width  = Context::GetWindowProperties().width;
			int height = Context::GetWindowProperties().height;

			// 텍스쳐 설명 구조체
			D3D11_TEXTURE2D_DESC textureDescPosNormDiffuse; // 포지션, 노말, 베이스 (32bit)

			// 포지션, 노말(position, nomral) - 32비트
			{ 
				textureDescPosNormDiffuse.Width = width;
				textureDescPosNormDiffuse.Height = height;
				textureDescPosNormDiffuse.MipLevels = 1;
				textureDescPosNormDiffuse.ArraySize = 1;
				textureDescPosNormDiffuse.Format =  DXGI_FORMAT_R32G32B32A32_FLOAT; // 32bit
				textureDescPosNormDiffuse.SampleDesc.Count = 1;
				textureDescPosNormDiffuse.SampleDesc.Quality = 0;
				textureDescPosNormDiffuse.Usage = D3D11_USAGE_DEFAULT;
				textureDescPosNormDiffuse.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				textureDescPosNormDiffuse.CPUAccessFlags = 0;
				textureDescPosNormDiffuse.MiscFlags = 0;
			}
			
			// 텍스쳐 생성
			m_textures[0] = nullptr;
			m_context->GetDevice()->CreateTexture2D(&textureDescPosNormDiffuse, NULL, &m_textures[0]);
			m_context->GetDevice()->CreateTexture2D(&textureDescPosNormDiffuse, NULL, &m_textures[1]);
			m_context->GetDevice()->CreateTexture2D(&textureDescPosNormDiffuse, NULL, &m_textures[2]);

		

			// 렌더 타겟 뷰 설명 구조체 (32bit)
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc32;
			ZeroMemory(&renderTargetViewDesc32, sizeof(renderTargetViewDesc32));
			{
				renderTargetViewDesc32.Format = textureDescPosNormDiffuse.Format;
				renderTargetViewDesc32.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc32.Texture2D.MipSlice = 0;
			}
	
			
			// 렌더 타겟 뷰 생성
			// pResource: 렌더링 뷰의 구조체 → 텍스처에 렌더링함
			// pDesc    : 렌더링 뷰의 정보 설정 구조체   
			// ppRTView : 생성한 뷰를 반환받는 포인터	
			m_renderTargetViews[0] = nullptr;
			m_context->GetDevice()->CreateRenderTargetView(m_textures[0], &renderTargetViewDesc32, &m_renderTargetViews[0]);
			m_context->GetDevice()->CreateRenderTargetView(m_textures[1], &renderTargetViewDesc32, &m_renderTargetViews[1]);
			m_context->GetDevice()->CreateRenderTargetView(m_textures[2], &renderTargetViewDesc32 , &m_renderTargetViews[2]);


		
			// 셰이더 리소스 뷰 설명 구조체
			// - 32
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc32DR;
			ZeroMemory(&shaderResourceViewDesc32DR, sizeof(shaderResourceViewDesc32DR));
			shaderResourceViewDesc32DR.Format = textureDescPosNormDiffuse.Format;
			shaderResourceViewDesc32DR.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc32DR.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc32DR.Texture2D.MipLevels = 1;


			// 셰이더 리소스 뷰 생성
			m_context->GetDevice()->CreateShaderResourceView(m_textures[0], &shaderResourceViewDesc32DR, &m_shaderResourceViews[0]);
			m_context->GetDevice()->CreateShaderResourceView(m_textures[1], &shaderResourceViewDesc32DR, &m_shaderResourceViews[1]);
			m_context->GetDevice()->CreateShaderResourceView(m_textures[2], &shaderResourceViewDesc32DR, &m_shaderResourceViews[2]);

			// Release texture
			for (int i = 0; i < BUFFER_COUNT; ++i)
				m_textures[i]->Release();

			// 깊이 / 굴곡 텍스쳐 설명 구조체
			D3D11_TEXTURE2D_DESC depthBufferDescDR;
			ZeroMemory(&depthBufferDescDR, sizeof(depthBufferDescDR));
			{
				depthBufferDescDR.Width = width;
				depthBufferDescDR.Height = height;
				depthBufferDescDR.MipLevels = 1;
				depthBufferDescDR.ArraySize = 1;
				depthBufferDescDR.Format = DXGI_FORMAT_R24G8_TYPELESS;
				depthBufferDescDR.SampleDesc.Count = 1;
				depthBufferDescDR.SampleDesc.Quality = 0;
				depthBufferDescDR.Usage = D3D11_USAGE_DEFAULT;
				depthBufferDescDR.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
				depthBufferDescDR.CPUAccessFlags = 0;
				depthBufferDescDR.MiscFlags = 0;
			}
			m_context->GetDevice()->CreateTexture2D(&depthBufferDescDR, 0, &m_depthStencilBuffer);

			// 깊이 / 스텐실 렌더 타겟생성
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescDR;
			ZeroMemory(&depthStencilViewDescDR, sizeof(depthStencilViewDescDR));
			{
				depthStencilViewDescDR.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilViewDescDR.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDescDR.Texture2D.MipSlice = 0;
			}
			//Create depth stencil view
			m_context->GetDevice()->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDescDR, &m_depthStencilView);
			// pResource: 뷰로 만들 DepthStecil 렌더
			// pDesc    : 뷰로 만들 DepthStecil 설정 구조체 (NULL은 기본 설정)
			// ppRTView : 생성된 DepthStencil을 반환받는 포인터


			// 셰이더 리소스 뷰 설명 구조체 (깊이버퍼)
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescDepth;
			ZeroMemory(&shaderResourceViewDescDepth, sizeof(shaderResourceViewDescDepth));
			shaderResourceViewDescDepth.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			shaderResourceViewDescDepth.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDescDepth.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDescDepth.Texture2D.MipLevels = 1;

			// 셰이더 리소스 뷰 생성 (깊이버퍼)
			m_context->GetDevice()->CreateShaderResourceView(m_depthStencilBuffer, &shaderResourceViewDescDepth, &m_shaderResourceViews[3]);
		}

		void DeferredBuffer::BindInternal()
		{
			//m_context->GetDeviceContext()->OMSetRenderTargets(3, m_renderTargetViews, Context::GetDepthStencilBuffer());
			m_context->GetDeviceContext()->OMSetRenderTargets(3, m_renderTargetViews, m_depthStencilView);
		}

		void DeferredBuffer::UnBindInternal()
		{
			m_context->Bind();
		}

	}
}
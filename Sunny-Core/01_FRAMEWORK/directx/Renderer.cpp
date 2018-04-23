#include "Renderer.h"

namespace sunny
{
	namespace directx
	{
		Renderer* Renderer::s_instance = nullptr;

		std::vector<ID3D11BlendState*>        Renderer::s_blendStates;
		std::vector<ID3D11DepthStencilState*> Renderer::s_depthStencilStates;

		Renderer::Renderer()
		{
			m_context = Context::GetContext();
		}

		void Renderer::Init()
		{
			s_instance = new Renderer();  
			s_instance->InitInternal(); 
		}

		void Renderer::InitInternal()
		{
			CreateBlendStates();
			CreateDepthStencilStates();

			SetDepthTesting(false);
			SetBlend(true);
		}

		void Renderer::ClearInternal(unsigned int buffer)
		{
			float color[4] = { 0.87f, 1.f, 1.f, 1.0f }; // 블랙

			if (buffer & RendererBufferType::RENDERER_BUFFER_COLOR)
				Context::GetDeviceContext()->ClearRenderTargetView(Context::GetBackBuffer(), color);

			if (buffer & RendererBufferType::RENDERER_BUFFER_DEPTH)
				Context::GetDeviceContext()->ClearDepthStencilView(Context::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
			if (buffer & RendererBufferType::RENDERER_BUFFER_DEFERRED)
			{
				Context::GetDeviceContext()->ClearRenderTargetView(GeometryBuffer::GetRenderTargeBuffer(GeometryTextureType::DIFFUSE), color);
				Context::GetDeviceContext()->ClearRenderTargetView(GeometryBuffer::GetRenderTargeBuffer(GeometryTextureType::NORMAL), color);
			}

		
			if (buffer & RendererBufferType::RENDERER_BUFFER_SHADOW)
				Context::GetDeviceContext()->ClearDepthStencilView(GeometryBuffer::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			
			
			if (buffer & RendererBufferType::RENDERER_BUFFER_DEBUG)
			{
				Context::GetDeviceContext()->ClearRenderTargetView(DebugBuffer::GetBuffer(), color);
				Context::GetDeviceContext()->ClearDepthStencilView(DebugBuffer::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
		}

		void Renderer::PresentInternal()
		{
			m_context->Present();
		}

		// 깊이/스텐실 상태를 출력 병합기 단계에 묶는다.
		void Renderer::SetDepthTestingInternal(bool enabled)
		{
			Context::GetDeviceContext()->OMSetDepthStencilState(enabled ? s_depthStencilStates[0] : s_depthStencilStates[1], NULL);
		}

		// 혼합 상태를 출력 병합기 단계에 묶는다.
		void Renderer::SetBlendInternal(bool enabled)
		{
			Context::GetDeviceContext()->OMSetBlendState(enabled ? s_blendStates[1] : s_blendStates[0], NULL, 0xffffffff);
		}

		// 혼합 설정 및 상태 생성
		void Renderer::CreateBlendStates()
		{
			// 혼합 설정들은 렌더 상태의 하나인 혼합 상태(blendstate)를 나타내는
			// ID3D11BlendState 인터페이스로 제어한다.
			// 이 인터페이스를 얻으려면 D3D11_BLEND_DESC 구조체를 채우고
			// ID3D11Device::CreateBlendState메서드를 호출해야 한다.

			// enabled : false
			{
				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

				desc.RenderTarget[0].BlendEnable = false;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				ID3D11BlendState* state;
				Context::GetDevice()->CreateBlendState(&desc, &state);

				s_blendStates.push_back(state);
			}

			// enabled : true
			{
				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
				
				desc.AlphaToCoverageEnable  = false;  
				// true로 설정하면 텍스처의 렌더링에 유용한 다중표본화 기법인 알파-포괄도(alpha-to-coverage) 변환이 활성화 된다.
				// 알파-포괄도 변환을 위해서는 다중표본화를 활성화해야 한다.(다중화 표뵨에 맞는 후면 버퍼와 깊이 버퍼를 생성해야한다.)
				
				desc.IndependentBlendEnable = false;
				// Direct3D 11에서는 장면을 최대 8개 까지의 렌더 대상들에 동시에 렌더링할 수 있따.
				// ture로 설정하면 각 렌더 대상마다 혼합을 개별적으로 수행할 수 있다.
				// false로 하면 모든 렌더 대상의 혼합 연산이 D3D11_BLEND_DSSC::RenderTarget 배열의 첫 원소에 있는 설정에 따라 동일하게 수행된다.

				// RenderTarget::D3D11_RENDER_TARGET_BLEND_DESC 원소 여덟 개짜리 배열로,
				// 이 배열의 i번째 원소는 다중 렌더 대상의 i번째 렌더 대상에 적용할 혼합 설정을 담은 구조체다.
				// IndependentBlendEnable을 false로 설정했다면 모든 렌더 대상은 RenderTarget[0]을 혼합해 사용한다.

				desc.RenderTarget[0].BlendEnable = true; // 혼합을 활성화 하려면 true
				
				desc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;     // RGB 성분 혼합의 원본 혼합 계수
				desc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA; // RGB 성분 혼합의 대상 혼합 계수
				desc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;        // RGB 성분 혼합 연산자
				desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;     // 알파 성분 혼합의 원본 혼합 계수
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;     // 알파 성분 혼합의 원본 혼합 계수
				desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;        // 알파 성분 혼합 연산자

				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 렌더 대상 쓰기 마스크

		
				ID3D11BlendState* state;
				Context::GetDevice()->CreateBlendState(&desc, &state);

				s_blendStates.push_back(state);
			}
		}

		// 깊이/스텐실 설정 및 상태 생성
		void Renderer::CreateDepthStencilStates()
		{
			// 스텐실 버퍼를 이용하면 후면 버퍼의 특정 영역으로의 렌더링을 방지할 수 있다.
			// 주어진 한 픽셀을 후면 버퍼에 기록할 것인지 아닌지를 스텐실 버퍼에 근거해서 판정하는 것을 스텐실 판정(stencil test)라고 부른다.

			// 스텐실 버퍼는(+깊이 버퍼) ID3D11DepthStencilState라는 렌더 상태 인터페이스로 제어한다.
			// ID3D11DepthStencilState 인터페이스를 생성하려면 우선 D3D11_DEPTH_STENCIL_DESC 구조체를 채워야 한다.
			
			// enabled : true
			{
				D3D11_DEPTH_STENCIL_DESC desc;
				
				desc.DepthEnable    = true;                                     // 깊이 버펄이 활성화(true), 비활성화(false)

				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;               // ZERO: 깊이 버퍼 쓰기(갱신) 방지, 깊이 판정 수행 | ALL: 깊이 버퍼 쓰기(갱신)도 활성화 
				desc.DepthFunc      = D3D11_COMPARISON_LESS;              // 깊이 판정에 쓰이는 비교 함수, D3D11_COMPAROSION_FUNC 열거형의 한 멤버
				
				desc.StencilEnable    = true;                                   // 스텐실 판정 활성화(true), 비활성화(false)
				desc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;        // 스텐실 판정 조건문   // 기본값은 그 어떤 비트도 막지 않는 마스크
				desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;       // 스텐실 버퍼를 갱신할때 특정 비트 값들이 갱신되지 않도록 하는 비트 마스크 // 기본값은 그 어떤 비트를 막지 않는 마스크 

				/* FrontFace: 전면 삼각형에 대한 스텍실 버퍼 적용 방식 */
				desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;      // 픽셀 단편이 스텍실 판정에 실패했을 때의 스텐실 버퍼의 갱신 방식
				desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;      // 픽셀 단편이 스텐실 판정은 통과되었으나 깊이 판정에 실패했을 때의 스텐실 버퍼의 갱신 방법
				desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;  // 픽셀 단편이 스텐실 판정과 깊이 판정을 모두 통과했을 때의 스텐실 버퍼의 갱신 방법 
				desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;    // 스텐실 판정 비교 함수를 결정하는 멤버 

				/* BackFace: 후면 삼각형에 대한 스텍실 버퍼 적용 방식 */
				desc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
				desc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
				desc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
				desc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;

				// D3D11_STENCIL_OP_KEEP     : 스텐실 버퍼를 변경하지 않도록 한다. 즉, 현재 잇는 값을 유지한다.
				// D3D11_STENCIL_OP_INCR_SAT : 스텐실 버퍼 항목을 증가한다. 최댓값을 넘기면 최댓값으로 한정한다. 
				
				ID3D11DepthStencilState* state;
				Context::GetDevice()->CreateDepthStencilState(&desc, &state);
			
				s_depthStencilStates.push_back(state);
			}
			
			// enabled : false
			{
				D3D11_DEPTH_STENCIL_DESC desc;

				desc.DepthEnable = false;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

				desc.StencilEnable = true;
				desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;       
				desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

				desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
				desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
				desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
				desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
				desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				ID3D11DepthStencilState* state;
				Context::GetDevice()->CreateDepthStencilState(&desc, &state);

				s_depthStencilStates.push_back(state);
			}
		}
	}
}
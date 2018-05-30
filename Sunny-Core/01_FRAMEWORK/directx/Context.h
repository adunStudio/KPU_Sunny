#pragma once

#include "../include.h"
#include "Common.h"
#include "../app/Window.h"

/*
DirectX 그래픽 프로그램은 기본적으로 다음과 같은 인터페이스 객체가 필요하다.

- 1. Direct3D 디바이스(Device)         인터페이스
- 2. 디바이스 컨텍스트(Device Context) 인터페이스
- 3. 스왑 체인(Swap Chain)             인터페이스
- 4. 렌더 타겟 뷰(Render Target View)  인터페이스

기본적으로 Direct3D 디바이스 인터페이스는 버퍼(정점 버퍼, 인덱스 버퍼, 상수 버퍼 등)의 생성, 리소스(텍스처 등)의 생성, 셰이더 생성, 래스터라이저 상태 생성, 리소스 뷰의 생성 등을 필요하다.

스왑 체인 인터페이스는 후면 버퍼의 내용을 출력하기 위해, 즉 프레젠테이션을 하는데 필요하다.

디바이스 컨텍스트 인터페이스는 렌더링과 관련된 작업을 위해 필요하다.
*/
namespace sunny
{
	namespace directx
	{
		class Context
		{
		private:
			static Context* s_context;

		public:
			static void Create(WindowProperties properties, void* deviceContext);

		public:
			// 디바이스 인터페이스에 대한 포인터다.  주로 리소스를 생성하는 데 필요하다.
			ID3D11Device* dev;

			// 디바이스 컨텍스트에 대한 포인터다.    주로 파이프라인을 설정하는 데 필요하다.
			ID3D11DeviceContext* devcon[2];

			// 스왑 체인 인터페이스에 대한 포인터다. 주로 디스플레이를 제어하는 데 필요하다.
			IDXGISwapChain* swapchain;

		private:
			D3D_FEATURE_LEVEL m_d3dFeatureLevel; // 기능 수준 레벨
			unsigned int  m_MSAAQuality;         // 멀티샘플링 품질 레벨
			bool m_MSAAEnabled;                  // 멀티샘플링 품질 지원 여부
			bool m_debugLayerEnabled;            // 디버그 계층을 활성화 하면 Direct3D는 VC++ 출력창에 디버그 메시지를 보낸다.

			ID3D11Texture2D* m_texture;

			ID3D11RenderTargetView* m_renderTargetView; // 화면에 보여지는 버퍼 객체(렌더 타겟 뷰) 인터페이스(swapchain에서 생성되고 관리된다.)
			ID3D11DepthStencilView* m_depthStencilView; // 깊이/스텐실 정보를 기반으로 View를 관리하는 객체의 인터페이스
			
			D3D11_VIEWPORT m_screenViewport;            // 뷰포트(윈도우에 그림이 그려지는 영역을  지정)

			ID3D11Texture2D* m_depthStencilBuffer;      // 일반적인 2D 이미지를 관리하는 객체 인터페이스(여기서는 깊이/스텐실 정보 저장용)

			WindowProperties m_properties;

			ID3D10Multithread* m_lock;
		public:
			Context(WindowProperties properties, void* deviceContext);
						
			void Present3D();

			void Present();

		private:		
			void InitD3D(HWND hWnd);

			void Resize();

			void BindInternal();

		public:
			/* 게터 */
			inline static Context* GetContext() { return s_context; }
			inline static Context* Get       () { return s_context; }

			inline static IDXGISwapChain* GetSwapChain() { return GetContext()->swapchain; }
			inline static ID3D11Device* GetDevice() { return GetContext()->dev; }
			inline static ID3D11DeviceContext* GetDeviceContext(int dimension = DIMENSION::D3) { return GetContext()->devcon[dimension]; }
			inline static ID3D11RenderTargetView* GetBackBuffer() { return GetContext()->m_renderTargetView; }
			inline static ID3D11DepthStencilView* GetDepthStencilBuffer() { return GetContext()->m_depthStencilView; }

			inline static const WindowProperties& GetWindowProperties() { return GetContext()->m_properties; }

			inline static void Bind() { return GetContext()->BindInternal(); };
		};
	}
}
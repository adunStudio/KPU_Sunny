#include "Context.h"


namespace sunny
{
	namespace directx
	{

#define ReleaseCOM(x)	\
	if (x)				\
	{					\
		x->Release();	\
		x = nullptr;	\
	}

		Context* Context::s_context = nullptr;

		void Context::Create(WindowProperties properties, void* deviceContext)
		{
			s_context = new Context(properties, deviceContext);
		}

		Context::Context(WindowProperties properties, void* deviceContext)
		: m_properties(properties), m_MSAAEnabled(true), m_debugLayerEnabled(true)
		{
			m_renderTargetView   = nullptr;
			m_depthStencilView   = nullptr;
			m_depthStencilBuffer = nullptr;

			/* 0. 초기화를 수행한다. */
			InitD3D((HWND)deviceContext);
		}

		void Context::InitD3D(HWND hWnd)
		{
			/* 1. 먼저 D3D11CreateDevice() 함수를 통하여 Direct3D 디바이스와 콘텍스트를 생성한다 */
			HRESULT hr = D3D11CreateDevice(
				NULL,                        // pAdapter  : 디스플레이 어댑터 포인터(2개 이상의 모니터를 사용할 경우) NULL이면 주 모니터를 사용
				D3D_DRIVER_TYPE_HARDWARE,    // DriverType: 하드웨어 가속 사용 
				NULL,                        // SoftWare  : 소프트웨어 구동기 -> 하드웨어를 사용할 경우 NULL
											 // Flags  : D3D11_CREATE_DEVICE_DEBUG          -> 디버그 계층 활성화(VC++ 출력 창에 디버그 메시지) 
											 //          D3D11_CREATE_DEVICE_SINGLETHREADED -> 싱글 쓰레드 일경우 성능 향상, CreateDeffedContext 호출은 실패한다.
				m_debugLayerEnabled ? D3D11_CREATE_DEVICE_DEBUG : D3D11_CREATE_DEVICE_SINGLETHREADED,
				NULL,                        // pFeatureLevels : [배열] 기능 수준(NULL 이면 최고 기능 수준)
				0,                           // FeatureLevel   : 위 배열의 원소 개수 (NULL 이면 0으로 지정)
				D3D11_SDK_VERSION,           // SDKVersion     : 버전 (항상 D3D11_SDK_VERSION)      
				&dev,                        // ppDevice       : 생성한 장치를 돌려준다.
				&m_d3dFeatureLevel,          // pFeatureLevel  : 기능 수준을 돌려준다.
				&devcon                      // ppImmediateContext: 생성된 장치 컨텍스트를 돌려준다.
			);

			// 멀티샘플링 품질 지원 여부(지원하는 퀄리티 레벨)를 알아낸다. (지원 X → 0)
			dev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MSAAQuality);

			/* 2. CreateSwapChain() 함수를 통하여 스왑 체인을 생성한다. */

			// 스왑체인 정보 구조체
			DXGI_SWAP_CHAIN_DESC scd;
			ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
			{
				// BufferDesc: 출력할 버퍼 형식
				scd.BufferDesc.Width = m_properties.width;          // 버퍼 가로 길이
				scd.BufferDesc.Height = m_properties.height;        // 버퍼 세로 길이
				scd.BufferDesc.RefreshRate.Numerator = 60;          // 리프레시(FPS) 분모
				scd.BufferDesc.RefreshRate.Denominator = 1;         // 리프레시(FPS) 분자
				scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 화면 출력 형식(색상 정보)

																	// SampleDesc: 멀티 샘플링 모드 형식
				scd.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;                     // 멀티샘플링 개수
				scd.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0; // 멀티샘플링 품질

																				  // BufferUsage: 생성되는 버퍼의 사용을 지정하는 플래그
				scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // 버퍼가 출력을 위한 렌더링 버퍼임을 명시

																	// BufferCount: 생성할 버퍼의 개수
				scd.BufferCount = 3; // 버퍼 개수

									 // OutputWindow: 출력할 위도우 핸들
				scd.OutputWindow = hWnd;

				// Windowed: 전체 모드 또는 윈도우 모드로 출력 할지를 결정
				scd.Windowed = !m_properties.fullscreen; // true -> 윈도우 모드

				// SwapEffect: swap이 일어난 후에 버퍼의 내용을 어떻게 할지 결정
				scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 버퍼의 내용을 더 이상 사용하지 않고 버림을 의미

				// 추가적인 플래그
				scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //  가장 적합한 디스플레이모드로 자동적으로 전환
			}

			IDXGIDevice* dxgiDevice = 0;
			IDXGIAdapter* dxgiAdapter = 0;
			IDXGIFactory* dxgiFactory = 0;

			// __uuidof: 인터페이스에 정의된 GUID를 얻어오는 역할을 한다.
			dev->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);      // (1) DirectX 장치에서 IDXGIDevice 를 얻어온다.
			dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);  // (2) IDXGIDevice 에서 어뎁터를 얻어온다.
			dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory); // (3) 어뎁터에서 IDXDIFactory 를 얻어온다.

																				  // 스왑체인을 생성한다.
																				  // (디바이스 포인터,  스왑체인 정보 구조체 포인터, 스왑체인 인터페이스를 받아오는 포인터)
			dxgiFactory->CreateSwapChain(dev, &scd, &swapchain);

			dxgiFactory->Release();
			dxgiAdapter->Release();
			dxgiDevice->Release();

			/* 렌더 타겟 뷰를 설정한다. */
			Resize();
		}

		void Context::Resize()
		{
			int width  = m_properties.width;
			int height = m_properties.height;

			ReleaseCOM(m_renderTargetView);
			ReleaseCOM(m_depthStencilView);
			ReleaseCOM(m_depthStencilBuffer);

			swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			/* 3. CreateRenderTargetView() 함수를 통해 렌더 타겟 뷰(백버퍼)를 생성한다. */

			ID3D11Texture2D* backBuffer;
			swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

			// 렌더 타겟 생성
			dev->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
			// pResource: 렌더링 뷰의 구조체             → 스왑체인의 백버퍼에 렌더링함
			// pDesc    : 렌더링 뷰의 정보 설정 구조체   → NULL은 기본 뷰로 만듬을 의미
			// ppRTView : 생성한 뷰를 반환받는 포인터

			backBuffer->Release();


			/* 4. 스텐실 텍스쳐를 생성한고 설정한다. */

			// 텍스쳐 정보 구조체 (스텐실)
			D3D11_TEXTURE2D_DESC depthStencilDesc;
			{
				depthStencilDesc.Width = width;              // 생성할 텍스쳐의 너비 길이
				depthStencilDesc.Height = height;            // 생성할 텍스쳐의 높이 길이
				depthStencilDesc.MipLevels = 1;              // 최대 밉맵 레벨을 지정 (밉맵: 한 장의 그림에 여러 스케일 장면을 포함한 텍스쳐)
				depthStencilDesc.ArraySize = 1;              // 텍스쳐 배열의 텍스쳐 갯수
				depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  // 깊이-depth로 24비트, 스텐실 값으로 8바이트를 사용하는 형식

				depthStencilDesc.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;                     // 멀티샘플링 개수
				depthStencilDesc.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0; // 멀티샘플링 품질

				// 텍스쳐의 읽기/쓰기 지정
				depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; // 기본 읽기 쓰기로 사용됨을 명시

				// 렌더링(렌더 파이프)에서 바인딩을 지정하는 플래그
				depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 깊이/스텐실 용도로 사용됨 의미

				// CPU의 접근을 지정하는 플래그
				depthStencilDesc.CPUAccessFlags = 0;          // 0은 CPU가 읽기/쓰기를 할 수 없을을 의미

				// 기타 속정 지정 플래그
				depthStencilDesc.MiscFlags = 0;               // 0은 사용하지 않음을 의미
			}

			// 깊이/스텐실 텍스쳐 생성 -> m_depthStencilBuffer
			dev->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer);
			// 생성할 텍스쳐 정보 구조체
			// 초기화 데이터(배열 포인터)
			// 생성된 2D 텍스쳐를 반환받는 포인터

			// 깊이/스텐실 렌더 타겟 생성
			dev->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_depthStencilView);
			// pResource: 뷰로 만들 DepthStecil 렌더
			// pDesc    : 뷰로 만들 DepthStecil 설정 구조체 (NULL은 기본 설정)
			// ppRTView : 생성된 DepthStencil을 반환받는 포인터


			/* 5. 렌더 타겟을 디바이스 컨텍스트의 출력 병합 단계에 연결한다. */
			devcon->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
			// NumViews : 뷰의 개수 (현재 1)
			// ppRenderTargetViews: 렌더 뷰의 포인터        (CreateRenderTargetView 에서 생성)
			// pDepthStencilView : DepthStencil 뷰의 포인터 (CreateDepthStencilView 에서 생성)


			/* 6. 뷰포트(윈도우에 글미이 그려지는 영역)를 설정하고 적용한다. */
			m_screenViewport.TopLeftX = 0;
			m_screenViewport.TopLeftY = 0;
			m_screenViewport.Width = (float)width;
			m_screenViewport.Height = (float)height;
			m_screenViewport.MinDepth = 0.0f;
			m_screenViewport.MaxDepth = 1.0f;
			devcon->RSSetViewports(1, &m_screenViewport);

			/* 7. 래스터라이저를 설정하고 적용한다.(도형이 어떻게 그려지는지에 대한 제어) &*/
			D3D11_RASTERIZER_DESC rasterDesc;
			{
				// 고급 기술을 위한 것이므로 그리 자주 쓰이지 않는다.
				// 자세한 설명은 SDK 문서를 참고 바랍니다.
				rasterDesc.AntialiasedLineEnable = false;
				rasterDesc.CullMode = D3D11_CULL_NONE;         // NONE: 삼각형 선별 비활성화, BACK: 후면 삼각형 제외, FRONT: 전면 삼각형 제외
				rasterDesc.DepthBias = 0;
				rasterDesc.DepthBiasClamp = 0.0f;
				rasterDesc.DepthClipEnable = true;
				rasterDesc.FillMode = D3D11_FILL_SOLID;        // 와이어 프레임 or 솔리드 프레임
				rasterDesc.FrontCounterClockwise = true;      // 시계 방향 삼각형 전면(true), 반시계 방향 삼각형 전면(false)
				rasterDesc.MultisampleEnable = false;
				rasterDesc.ScissorEnable = false;
				rasterDesc.SlopeScaledDepthBias = 0.0f;
			}

			ID3D11RasterizerState* rs;
			dev->CreateRasterizerState(&rasterDesc, &rs);
			devcon->RSSetState(rs);

			ReleaseCOM(rs);
		}

		void Context::Present()
		{
			// 이중 버퍼링을 사용할 때 후면 버퍼의 내용을 전면 버퍼로 복사하는 것을 프레젠테이션이라고 한다.
			swapchain->Present(m_properties.vsync, 0);
		}
	}
}
#include "Texture2D.h"

namespace sunny
{
	namespace directx
	{
		/* STATIC */
		DXGI_FORMAT Texture2D::TextureFormatToD3D(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB:
			case TextureFormat::RGBA:
				return DXGI_FORMAT_R8G8B8A8_UNORM;

			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}


		/* 생성자 */

		Texture2D::Texture2D(unsigned int width, unsigned int height, DIMENSION dimension)
			: m_fileName("NULL"), m_width(width), m_height(height), m_dimension(dimension)
		{
			Load();
		}

		Texture2D::Texture2D(const std::string& name, const std::string& filename, DIMENSION dimension)
			: m_fileName(filename), m_name(name), m_dimension(dimension)
		{
			Load();
		}

		Texture2D::Texture2D(const std::string& filename, DIMENSION dimension)
			: m_fileName(filename), m_name(filename), m_dimension(dimension)
		{
			Load();
		}

		Texture2D::~Texture2D()
		{

		}


		/* LOAD */

		void Texture2D::Load()
		{
			unsigned char* data = nullptr;

			if (m_fileName != "NULL")
			{
				data = utils::LoadSunnyImage(m_fileName, &m_width, &m_height, &m_bitsPerPixel, true);
			}

			bool generateMips = (data != nullptr);

			unsigned int stride = 4;

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = data;                         // 초기화 데이터에 대한 포인터
			initData.SysMemPitch = stride * m_width;             // 텍스처의 특정 줄의 시작에서 다음 줄 시작까지의 바이트 수
			initData.SysMemSlicePitch = m_width * m_height * stride;  // 특정 깊이 레벨의 시작에서 다음 깊이 레벨까지의 바이트 수(3차원이 아닐 경우 의미 없다.)

			D3D11_SUBRESOURCE_DATA* initDataPtr = nullptr;

			unsigned int mipLevels = 1;

			// 밉맵
			if (generateMips)
			{
				unsigned int width = m_width;
				unsigned int height = m_height;

				while (width > 1 && height > 1)
				{
					// 2배씩 작아진다.
					width = max(width / 2, 1u);
					height = max(height / 2, 1u);
					++mipLevels;
				}
			}
			else
			{
				if (data)
					initDataPtr = &initData;
			}

			DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;


			// 텍스쳐 정보 구조체
			ZeroMemory(&m_desc, sizeof(D3D11_TEXTURE2D_DESC));

			m_desc.Width = m_width; // 생성할 텍스쳐의 너비 길이
			m_desc.Height = m_height; // 생성할 텍스쳐의 높이 길이
			m_desc.MipLevels = mipLevels; // // 최대 밉맵 레벨을 지정 (밉맵: 한 장의 그림에 여러 스케일 장면을 포함한 텍스쳐)
			m_desc.ArraySize = 1;              // 텍스쳐 배열의 텍스쳐 갯수
			m_desc.Format = format;
			m_desc.CPUAccessFlags = 0;          // 0은 CPU가 읽기/쓰기를 할 수 없을을 의미
			m_desc.Usage = generateMips ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
			m_desc.CPUAccessFlags = m_desc.Usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
			m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;// 텍스처를 파이프라인 단계에 어떻게 연결 하는지 설정
			if (generateMips)
				m_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			m_desc.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
			m_desc.SampleDesc.Count = 1; // 멀티샘플링 개수
			m_desc.SampleDesc.Quality = 0;// 멀티샘플링 품질

			Context::GetDevice()->CreateTexture2D(&m_desc, initDataPtr, &m_texture);
			// 생성할 텍스쳐 정보 구조체
			// 초기화 데이터(배열 포인터)
			// 생성된 2D 텍스쳐를 반환받는 포인터

			/* 텍스쳐에 대한 셰이더 자원 뷰를 생성한다. */

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			srvDesc.Format = m_desc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = m_desc.MipLevels;

			Context::GetDevice()->CreateShaderResourceView(m_texture, &srvDesc, &m_resourceView);

			if (generateMips)
			{
				Context::GetDeviceContext(m_dimension)->UpdateSubresource(m_texture, 0, nullptr, initData.pSysMem, initData.SysMemPitch, initData.SysMemSlicePitch);
				Context::GetDeviceContext(m_dimension)->GenerateMips(m_resourceView);
			}
			
			m_desc.Usage = D3D11_USAGE_DEFAULT;
			m_desc.CPUAccessFlags = 0;
			m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			ZeroMemory(&m_samplerDesc, sizeof(D3D11_SAMPLER_DESC));
			m_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			m_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			m_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			m_samplerDesc.MinLOD = 0;
			m_samplerDesc.MaxLOD = 11;
			m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			m_samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			m_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			Context::GetDevice()->CreateSamplerState(&m_samplerDesc, &m_samplerState);

			if (data != nullptr)
				delete[] data;
		}


		/*     */

		void Texture2D::Bind(unsigned int slot) const
		{
			Context::GetDeviceContext(m_dimension)->PSSetShaderResources(slot, 1, &m_resourceView);
			Context::GetDeviceContext(m_dimension)->PSSetSamplers(slot, 1, &m_samplerState);
		}

		void Texture2D::UnBind(unsigned int slot) const
		{
			ID3D11ShaderResourceView* rv = nullptr;

			Context::GetDeviceContext(m_dimension)->PSSetShaderResources(slot, 1, &rv);
		}

		void Texture2D::SetData(const void* pixels)
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

			Context::GetDeviceContext(m_dimension)->Map(m_texture, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);

			for (unsigned int i = 0; i < m_width * m_height * GetStrideFromFormat(TextureFormat::RGBA); i += 4)
			{
				((byte*)msr.pData)[i + 0] = 0xff;
				((byte*)msr.pData)[i + 1] = 0xff;
				((byte*)msr.pData)[i + 2] = 0xff;
				((byte*)msr.pData)[i + 3] = ((byte*)pixels)[i / 2 + 1];
			}

			Context::GetDeviceContext(m_dimension)->Unmap(m_texture, NULL);
		}
	}
}
#include "TextureCube.h"

namespace sunny
{
	namespace directx
	{
		
		TextureCube::TextureCube(const std::string& name, const std::string* files, int mips)
		{
			m_name = name;
			m_files = new std::string[mips];

			for (unsigned int i = 0; i < mips; ++i)
				m_files[i] = files[i];

			LoadFromVCross(mips);
		}

		TextureCube::~TextureCube()
		{

		}

		void TextureCube::Bind(unsigned int slot) const
		{
			Context::GetDeviceContext(DIMENSION::D3)->PSSetShaderResources(slot, 1, &m_resourceView);
			Context::GetDeviceContext(DIMENSION::D3)->PSSetSamplers(slot, 1, &m_samplerState);
		}

		void TextureCube::UnBind(unsigned int slot) const
		{
			ID3D11ShaderResourceView* rv = nullptr;
			Context::GetDeviceContext(DIMENSION::D3)->PSSetShaderResources(slot, 1, &rv);
		}

		void TextureCube::LoadFromVCross(int mips)
		{
			unsigned int srcWidth, srcHeight;
			unsigned int bits;

			unsigned char*** cubeTextureData = new unsigned char**[mips];

			for (unsigned int i = 0; i < mips; ++i)
				cubeTextureData[i] = new unsigned char*[6];

			unsigned int* faceWidths   = new unsigned int[mips];
			unsigned int* faceHeights  = new unsigned int[mips];
		
			for (unsigned int m = 0; m < mips; ++m)
			{
				unsigned char* data = utils::LoadSunnyImage(m_files[m], &srcWidth, &srcHeight, &bits, true);
			
				unsigned int stride = bits / 8;

				std::cout << *data << std::endl;
				std::cout << "mip: " << m << ", stride: " << stride << std::endl;
				std::cout << "width: " << srcWidth << ", height: " << srcHeight << std::endl;

				unsigned int face = 0;
				unsigned int faceWidth = srcWidth / 3;
				unsigned int faceHeight = srcHeight / 4;

				faceWidths[m]  = faceWidth;
				faceHeights[m] = faceHeight;

				for (unsigned int cy = 0; cy < 4; ++cy)
				{
					for (unsigned int cx = 0; cx < 3; ++cx)
					{
						if ((cy == 0 || cy == 2 || cy == 3) && cx != 1)
								continue;

						cubeTextureData[m][face] = new unsigned char[faceWidth * faceHeight * stride];

						unsigned int index = 0;

						for (unsigned int y = 0; y < faceHeight; ++y)
						{
							unsigned int offset = y;
							if (face == 5) offset = faceHeight - (y + 1);

							unsigned int yp = cy * faceHeight + offset;

							for (unsigned int x = 0; x < faceWidth; ++x)
							{
								offset = x;
								if (face == 5) offset = faceWidth - (x + 1);

								unsigned xp = cx * faceWidth + offset;

								cubeTextureData[m][face][(x + y * faceWidth) * stride + 0] = data[(xp + yp * srcWidth) * stride + 0];
								cubeTextureData[m][face][(x + y * faceWidth) * stride + 1] = data[(xp + yp * srcWidth) * stride + 1];
								cubeTextureData[m][face][(x + y * faceWidth) * stride + 2] = data[(xp + yp * srcWidth) * stride + 2];
								if (stride >= 4)
									cubeTextureData[m][face][(x + y * faceWidth) * stride + 3] = data[(xp + yp * srcWidth) * stride + 3];
							}
						}
						face++;
					}
				}

				delete[] data;

				D3D11_TEXTURE2D_DESC texDesc;
				texDesc.Width              = faceWidths[0];
				texDesc.Height             = faceHeights[0];
				texDesc.MipLevels          = mips;
				texDesc.ArraySize          = 6;
				texDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
				texDesc.CPUAccessFlags     = 0;          // 0은 CPU가 읽기/쓰기를 할 수 없을을 의미
				texDesc.SampleDesc.Count   = 1;          // 멀티샘플링 개수
				texDesc.SampleDesc.Quality = 0;          // 멀티샘플링 품질
				texDesc.Usage              = D3D11_USAGE_DEFAULT;
				texDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;// 텍스처를 파이프라인 단계에 어떻게 연결 하는지 설정
				texDesc.MiscFlags          = D3D11_RESOURCE_MISC_TEXTURECUBE;
				
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				srvDesc.Format = texDesc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				srvDesc.TextureCube.MipLevels = texDesc.MipLevels;
				srvDesc.TextureCube.MostDetailedMip = 0;

				D3D11_SUBRESOURCE_DATA* pData = new D3D11_SUBRESOURCE_DATA[6 * mips];

				unsigned int result = 0;
				unsigned int index = 0;
				unsigned int faceOrder[6] = { 3, 1, 0, 4, 2, 5 };

				for (int f = 0; f < 6; ++f)
				{
					unsigned fi = faceOrder[f];
					for (int m = 0; m < mips; ++m)
					{
						pData[index].pSysMem = cubeTextureData[m][fi];
						pData[index].SysMemPitch = faceWidths[m] * 4;
						pData[index].SysMemSlicePitch = faceWidths[m] * faceHeights[m] * 4;
						index++;
					}
				}

				m_texture = nullptr;
				Context::GetDevice()->CreateTexture2D(&texDesc, pData, &m_texture);
				Context::GetDevice()->CreateShaderResourceView(m_texture, &srvDesc, &m_resourceView);
			
				delete[] pData;

				/*for (int m = 0; m < mips; ++m)
				{
					for (int f = 0; f < 6; ++f)
						delete[] cubeTextureData[m][f];

					delete[] cubeTextureData[m];
				}

				delete[] cubeTextureData;*/

				ZeroMemory(&m_samplerDesc, sizeof(D3D11_SAMPLER_DESC));
				m_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				m_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				m_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				m_samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				m_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

				Context::GetDevice()->CreateSamplerState(&m_samplerDesc, &m_samplerState);
			}
		}
	}
}
#pragma once

#include "../include.h"
#include "Common.h"
#include "Texture.h"
#include "Context.h"

namespace sunny
{
	namespace directx
	{
		class Texture2D : public Texture
		{
		public:
			static DXGI_FORMAT  TextureFormatToD3D(TextureFormat format);

		private:
			std::string m_name;
			std::string m_fileName;

			unsigned int m_handle;

			unsigned int m_width, m_height;

			unsigned int m_bitsPerPixel;

			D3D11_TEXTURE2D_DESC m_desc;
			ID3D11Texture2D* m_texture;
			ID3D11ShaderResourceView* m_resourceView;
			ID3D11SamplerState* m_samplerState;
			D3D11_SAMPLER_DESC m_samplerDesc;

			DIMENSION m_dimension;

		private:
			void Load();

		public:
			Texture2D(unsigned int width, unsigned int height,              DIMENSION dimension = DIMENSION::D3);
			Texture2D(const std::string& name, const std::string& filename, DIMENSION dimension = DIMENSION::D3);
			Texture2D(const std::string& filename,                          DIMENSION dimension = DIMENSION::D3);

			~Texture2D() override;

			void Bind(unsigned int slot = 0)   const override;
			void UnBind(unsigned int slot = 0) const override;

			void SetData(const void* pixels);

			unsigned int GetWidth()  const { return m_width; }
			unsigned int GetHeight() const { return m_height; }
			
			inline const std::string& GetName()     const override { return m_name; }
			inline const std::string& GetFilePath() const override { return m_fileName; }
		};
	}
}
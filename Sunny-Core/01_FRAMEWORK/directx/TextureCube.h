#pragma once

#include "../include.h"
#include "Common.h"
#include "Texture.h"
#include "Context.h"

namespace sunny
{
	namespace directx
	{
		class TextureCube : public Texture
		{
		private:
			unsigned int m_width, m_height;

			std::string m_name;

			std::string* m_files;

			unsigned int m_bits;

			D3D11_TEXTURE2D_DESC m_desc;
			ID3D11Texture2D* m_texture;
			ID3D11ShaderResourceView* m_resourceView;
			ID3D11SamplerState* m_samplerState;
			D3D11_SAMPLER_DESC m_samplerDesc;

		public:
			TextureCube(const std::string& name, const std::string* files, int mips);
			~TextureCube();

			void Bind(unsigned int slot = 0) const override;
			void UnBind(unsigned int slot = 0) const override;
		
			inline const std::string& GetName()     const override { return m_name; }
			inline const std::string& GetFilePath() const override { return m_files[0]; }

		private:
			void LoadFromVCross(int mips);
		};
	}
}
#include "ShadowMap.h"

namespace sunny
{
	namespace graphics
	{

		ShadowMap::ShadowMap()
		{
			Init();
		}

		void ShadowMap::Init()
		{
			m_buffer = directx::ShadowBuffer::GetShadowBuffer();
		}

		void ShadowMap::SetShadowMapTexture()
		{
			ID3D11ShaderResourceView* rv = m_buffer->GetShaderResource();
			directx::Context::GetDeviceContext()->PSSetShaderResources(SHADOW_MAP_INDEX, 1, &rv);
		}

		void ShadowMap::Bind()
		{
			m_buffer->Bind();
		}

		void ShadowMap::UnBInd()
		{
			m_buffer->UnBind();
		}
	}
}
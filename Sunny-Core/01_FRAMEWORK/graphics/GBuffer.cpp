#include "GBuffer.h"

namespace sunny
{
	namespace graphics
	{
		GBuffer::GBuffer()
		{
			Init();
		}

		GBuffer::~GBuffer()
		{

		}

		void GBuffer::Init()
		{
			m_buffer = directx::DeferredBuffer::GetDeferredBuffer();
		}

		void GBuffer::SetGBufferTexture(TextureType type)
		{
			int index = static_cast<int>(type);
			ID3D11ShaderResourceView* rv = directx::DeferredBuffer::GetShaderResource(index);
			directx::Context::GetDeviceContext()->PSSetShaderResources(index, 1, &rv);
		}

		void GBuffer::SetGBufferSampler()
		{
			ID3D11SamplerState* sampler = directx::DeferredBuffer::GetSamplerState();
			directx::Context::GetDeviceContext()->PSSetSamplers(0, 1, &sampler);
		}

		void GBuffer::Bind()
		{
			m_buffer->Bind();
		}

		void GBuffer::UnBind()
		{
			m_buffer->UnBind();


		}

		void GBuffer::Draw()
		{
			static const UINT     stride = 0;
			static const UINT     offset = 0;
			static ID3D11Buffer* nothing = 0;

			directx::Context::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			directx::Context::GetDeviceContext()->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
			directx::Context::GetDeviceContext()->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

			directx::Context::GetDeviceContext()->Draw(3, 0);
		}
	}
}
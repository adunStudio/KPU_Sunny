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
			m_buffer = directx::GeometryBuffer::GetGeometryBuffer();
		}

		void GBuffer::SetGBuffer(unsigned int buffer)
		{
			if (buffer & GBufferType::DEFERRED)
			{
				ID3D11ShaderResourceView * diffuse_rv = directx::GeometryBuffer::GetShaderResource(GeometryTextureType::DIFFUSE);
				ID3D11ShaderResourceView * normal_rv  = directx::GeometryBuffer::GetShaderResource(GeometryTextureType::NORMAL);
				
				ID3D11SamplerState* sampler = directx::GeometryBuffer::GetSamplerState(GeometryTextureType::DIFFUSE);

				directx::Context::GetDeviceContext()->PSSetShaderResources(GBufferTextureIndex::DIFFUSE, 1, &diffuse_rv);
				directx::Context::GetDeviceContext()->PSSetShaderResources(GBufferTextureIndex::NORMAL , 1, &normal_rv);
				directx::Context::GetDeviceContext()->PSSetSamplers(GBufferSapmaerIndex::DEFERRED_SAMPLER, 1, &sampler);
			}

			if (buffer & GBufferType::SHADOWMAP)
			{
				ID3D11ShaderResourceView * shadow_rv = directx::GeometryBuffer::GetShaderResource(GeometryTextureType::DEPTH);

				ID3D11SamplerState* sampler = directx::GeometryBuffer::GetSamplerState(GeometryTextureType::DEPTH);

				directx::Context::GetDeviceContext()->PSSetShaderResources(GBufferTextureIndex::SHADOW, 1, &shadow_rv);
				directx::Context::GetDeviceContext()->PSSetSamplers(GBufferSapmaerIndex::SHADOW_SMAPLER, 1, &sampler);
			}
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
#include "Material.h"

namespace sunny
{
	namespace graphics
	{
		Material::Material(directx::Shader* shader) : m_shader(shader)
		{
			AllocateStorage();
			m_resources = &shader->GetResources();
		}

		Material::~Material()
		{
			delete[] m_VSUserUniformBuffer;
			delete[] m_PSUserUniformBuffer;
		}

		void Material::Bind()
		{
			m_shader->Bind();

			if (m_VSUserUniformBuffer)
				m_shader->SetVSUserUniformBuffer(m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
			if (m_PSUserUniformBuffer)
				m_shader->SetPSUserUniformBuffer(m_PSUserUniformBuffer, m_PSUserUniformBufferSize);

			for (auto texture : m_textures)
			{
				if (texture) texture->Bind();
			}
		}

		void Material::UnBind()
		{
			for (auto texture : m_textures)
			{
				if (texture) texture->UnBind();
			}
		}

		void Material::SetUniformData(const std::string& uniform, unsigned char* data)
		{
			unsigned char* buffer = nullptr;

			directx::ShaderUniformDeclaration* declaration = FindUniformDeclaration(uniform, &data);

			if (!buffer)
			{
				// Debug System
				std::cout << "MaterialInstance::SetUniformData" << std::endl;
				exit(1);
			}

			memcpy(buffer + declaration->GetOffset(), data, declaration->GetSize());
		}

		void Material::SetTexture(const std::string& name, directx::Texture* texture)
		{
			directx::ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);
		
			if (!declaration)
			{
				std::cout << "Material::SetTexture" << std::endl;
				exit(1);
			}

			unsigned int slot = declaration->GetRegister();

			if (m_textures.size() <= slot)
				m_textures.resize(slot + 1);

			m_textures[slot] = texture;
		}

		void Material::AllocateStorage()
		{
			m_VSUserUniformBuffer = nullptr;
			m_VSUserUniformBufferSize = 0;
			m_PSUserUniformBuffer = nullptr;
			m_PSUserUniformBufferSize = 0;
			m_VSUserUniforms = nullptr;
			m_PSUserUniforms = nullptr;

			// 버텍스 상수 버퍼
			const directx::ShaderUniformBufferDeclaration* vsBuffer = m_shader->GetVSUserUniformBuffer();
			if (vsBuffer)
			{
				m_VSUserUniformBufferSize = vsBuffer->GetSize();
				m_VSUserUniformBuffer = new unsigned char[m_VSUserUniformBufferSize];
				memset(m_VSUserUniformBuffer, 0, m_VSUserUniformBufferSize);
				m_VSUserUniforms = &vsBuffer->GetUniformDeclarations();
			}

			// 픽셀 상수 버퍼
			const directx::ShaderUniformBufferDeclaration* psBuffer = m_shader->GetPSUserUniformBuffer();
			if (psBuffer)
			{
				m_PSUserUniformBufferSize = psBuffer->GetSize();
				m_PSUserUniformBuffer = new unsigned char[m_PSUserUniformBufferSize];
				memset(m_PSUserUniformBuffer, 0, m_PSUserUniformBufferSize);
				m_PSUserUniforms = &psBuffer->GetUniformDeclarations();
			}
		}

		directx::ShaderUniformDeclaration*  Material::FindUniformDeclaration(const std::string& name, unsigned char** outBuffer)
		{
			// 버텍스 셰이더에서 찾는다.
			if (m_VSUserUniforms)
			{
				for (directx::ShaderUniformDeclaration* uniform : *m_VSUserUniforms)
				{
					if (uniform->GetName() == name)
					{
						*outBuffer = m_VSUserUniformBuffer;
						
						return uniform;
					}
				}
			}

			// 픽셀 셰이더에서 찾는다.
			if (m_PSUserUniforms)
			{
				for (directx::ShaderUniformDeclaration* uniform : *m_PSUserUniforms)
				{
					if (uniform->GetName() == name)
					{
						*outBuffer = m_PSUserUniformBuffer;

						return uniform;
					}
				}
			}

			return nullptr;
		}

		directx::ShaderResourceDeclaration* Material::FindResourceDeclaration(const std::string& name)
		{
			for (directx::ShaderResourceDeclaration* resource : *m_resources)
			{
				if (resource->GetName() == name) return resource;
			}

			return nullptr;
		}
	}
}
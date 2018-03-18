#include "MaterialInstance.h"

namespace sunny
{
	namespace graphics
	{
		MaterialInstance::MaterialInstance(Material* material) : m_material(material)
		{
			AllocateStorage();

			memcpy(m_VSUserUniformBuffer, m_material->m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
			memcpy(m_PSUserUniformBuffer, m_material->m_PSUserUniformBuffer, m_PSUserUniformBufferSize);

			m_resources = &m_material->m_shader->GetResources();
		}

		void MaterialInstance::Bind()
		{
			m_material->Bind();

			if (m_VSUserUniformBuffer)
				m_material->m_shader->SetVSUserUniformBuffer(m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
			if (m_PSUserUniformBuffer)
				m_material->m_shader->SetPSUserUniformBuffer(m_PSUserUniformBuffer, m_PSUserUniformBufferSize);

			for (auto texture : m_textures)
			{
				if (texture) texture->Bind();
			}
		}

		void MaterialInstance::UnBind()
		{
			m_material->UnBind();

			for (auto texture : m_textures)
			{
				if (texture) texture->UnBind();
			}
		}

		void MaterialInstance::SetUniformData(const std::string& uniform, unsigned char* data)
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

		void MaterialInstance::SetTexture(const std::string& name, directx::Texture* texture)
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

		void MaterialInstance::AllocateStorage()
		{
			// 버텍스 상수 버퍼
			const directx::ShaderUniformBufferDeclaration* vsBuffer = m_material->m_shader->GetVSUserUniformBuffer();
			if (vsBuffer)
			{
				m_VSUserUniformBufferSize = vsBuffer->GetSize();
				m_VSUserUniformBuffer = new unsigned char[m_VSUserUniformBufferSize];
				m_VSUserUniforms = &vsBuffer->GetUniformDeclarations();
			}

			// 픽셀 상수 버퍼
			const directx::ShaderUniformBufferDeclaration* psBuffer = m_material->m_shader->GetPSUserUniformBuffer();
			if (psBuffer)
			{
				m_PSUserUniformBufferSize = psBuffer->GetSize();
				m_PSUserUniformBuffer = new unsigned char[m_PSUserUniformBufferSize];
				m_PSUserUniforms = &psBuffer->GetUniformDeclarations();
			}
		}

		directx::ShaderUniformDeclaration*  MaterialInstance::FindUniformDeclaration(const std::string& name, unsigned char** outBuffer)
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

		directx::ShaderResourceDeclaration* MaterialInstance::FindResourceDeclaration(const std::string& name)
		{
			for (directx::ShaderResourceDeclaration* resource : *m_resources)
			{
				if (resource->GetName() == name) return resource;
			}

			return nullptr;
		}
	}
}
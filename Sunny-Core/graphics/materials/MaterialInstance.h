#pragma once

#include "../../sunny.h"
#include "Material.h"

namespace sunny
{
	namespace graphics
	{
		class MaterialInstance
		{
		private:
			Material* m_material;

			unsigned char* m_VSUserUniformBuffer;
			unsigned int   m_VSUserUniformBufferSize;
			unsigned char* m_PSUserUniformBuffer;
			unsigned int   m_PSUserUniformBufferSize;

			std::vector<directx::Texture*> m_textures;

			const directx::ShaderUniformList*  m_VSUserUniforms;
			const directx::ShaderUniformList*  m_PSUserUniforms;
			const directx::ShaderResourceList* m_resources;

		public:
			MaterialInstance(Material* material);

			inline Material* GetMaterial() const { return m_material; }

			void Bind();
			void UnBind();

			void SetUniformData(const std::string& uniform, unsigned char* data);
			void SetTexture(const std::string& name, directx::Texture* texture);

			template<typename T>
			void SetUniform(const std::string& name, const T& data)
			{
				unsigned char* buffer;

				directx::ShaderUniformDeclaration* declaration = FindUniformDeclaration(name, &buffer);

				if (!declaration)
				{
					// Debug System
					std::cout << "Could not find uniform with name '" << name << "'!" << std::endl;
					exit(0);
				}

				memcpy(buffer + declaration->GetOffset(), &data, declaration->GetSize());
			}

		private:
			void AllocateStorage();
			directx::ShaderUniformDeclaration*  FindUniformDeclaration(const std::string& name, unsigned char** outBuffer = nullptr);
			directx::ShaderResourceDeclaration* FindResourceDeclaration(const std::string& name);
		};
	}
}
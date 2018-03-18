#pragma once

#include "../../sunny.h"

#include "../../directx/Shader.h"
#include "../../directx/Texture.h"
#include "../../directx/Texture2D.h"


namespace sunny
{
	namespace graphics
	{
		class MaterialInstance;

		class Material
		{
		private:
			friend MaterialInstance;

		protected:
			directx::Shader* m_shader;

			unsigned char* m_VSUserUniformBuffer;
			unsigned int   m_VSUserUniformBufferSize;
			unsigned char* m_PSUserUniformBuffer;
			unsigned int   m_PSUserUniformBufferSize;

			std::vector<directx::Texture*> m_textures;

			const directx::ShaderUniformList*  m_VSUserUniforms;
			const directx::ShaderUniformList*  m_PSUserUniforms;
			const directx::ShaderResourceList* m_resources;

		public:
			Material(directx::Shader* shader);
			~Material();

			void Bind();
			void UnBind();
			
			void SetUniformData(const std::string& uniform, unsigned char* data);
			void SetTexture(const std::string& name, directx::Texture* texture);

			inline directx::Shader* GetShader() { return m_shader; }

			template<typename T>
			void SetUniform(const std::string& name, const T& data)
			{
				unsigned char* buffer;

				directx::ShaderUniformDeclaration* declaration = FindUniformDeclaration(name, &buffer);
			
				if (!declaration)
				{
					// Debug System
					std::cout << "Material::SetUnifor() - Could not find uniform with name '" << name  << "'!" << std::endl;
					exit(0);
				}

				memcpy(buffer + declaration->GetOffset(), &data, declaration->GetSize());
			}

			template<typename T>
			const T* GetUniform(const directx::ShaderUniformDeclaration* uniform) const
			{
				return (T*)&m_uniformData[uniform->GetOffset()];
			}

		protected:
			void AllocateStorage();
			directx::ShaderUniformDeclaration*  FindUniformDeclaration (const std::string& name, unsigned char** outBuffer = nullptr);
			directx::ShaderResourceDeclaration* FindResourceDeclaration(const std::string& name);
		};
	}
}
#include "PBRMaterial.h"

namespace sunny
{
	namespace graphics
	{
		directx::Texture2D* PBRMaterial::s_PreintegratedFG = nullptr;

		PBRMaterial::PBRMaterial(directx::Shader* shader) : Material(shader)
		{
			if (s_PreintegratedFG == nullptr)
				s_PreintegratedFG = directx::Texture2D::CreateFromFile("PreintegratedFG", "resource/pbr/PreintegratedFG.bmp");

			SetTexture("u_PreintegratedFG", s_PreintegratedFG);
			SetUniform("u_UsingNormalMap", 0.0f);

			// 물리 기반 렌더링을 위한 기본 세팅
			SetUniform("u_UsingAlbedoMap", 0.0f);
			SetUniform("u_AlbedoColor", maths::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			SetUniform("u_SpecularColor", maths::vec3(1.0f, 1.0f, 1.0f));
			SetUniform("u_UsingSpecularMap", 0.0f);

			SetUniform("u_GlossColor", 0.8f);
			SetUniform("u_UsingGlossMap", 0.0f);

			SetUniform("u_UsingNormalMap", 0.0f);
		}

		PBRMaterial::~PBRMaterial()
		{
			delete m_shader;
		}

		//void PBRMaterial::SetEnviromentMap(directx::TextureCube* texture);

		// PBR Statics
		void PBRMaterial::SetAlbedo(const maths::vec4& color)
		{
			SetUniform("u_AlbedoColor", color);
			SetUniform("u_UsingAlbedoMap", 0.0f);
		}

		void PBRMaterial::SetSpecular(const maths::vec3& color)
		{
			SetUniform("u_SpecularColor", color);
			SetUniform("u_UsingSpecularMap", 0.0f);
		}

		void PBRMaterial::SetGloss(float value)
		{
			SetUniform("u_GlossColor", value);
			SetUniform("u_UsingGlossMap", 0.0f);
		}

		void PBRMaterial::UsingNormalMap(bool value)
		{
			SetUniform("u_UsingNormalMap", value ? 1.0f : 0.0f);
		}

		// PBR Maps
		void PBRMaterial::SetAlbedoMap(directx::Texture2D* texture)
		{
			SetTexture("u_AlbedoMap", texture);
			SetUniform("u_UsingAlbedoMap", 1.0f);
		}

		void PBRMaterial::SetSpecularMap(directx::Texture2D* texture)
		{
			SetTexture("u_SpecularMap", texture);
			SetUniform("u_UsingSpecularMap", 1.0f);
		}

		void PBRMaterial::SetNormalMap(directx::Texture2D* texture)
		{
			SetTexture("u_NormalMap", texture);
			SetUniform("u_UsingNormalMap", 1.0f);
		}

		void PBRMaterial::SetGlossMap(directx::Texture2D* texture)
		{
			SetUniform("u_GlossMap", texture);
			SetUniform("u_UsingGlossMap", 1.0f);
		}

		directx::Texture* PBRMaterial::GetAlbedoMap()
		{
			directx::ShaderResourceDeclaration* declaration = FindResourceDeclaration("u_AlbedoMap");

			if (!declaration)
			{
				// Dubug System
				std::cout << "PBRMaterial::GetAlbedoMap()" << std::endl;
				exit(1);
			}

			unsigned int slot = declaration->GetRegister();

			return m_textures.size() > slot ? m_textures[slot] : nullptr;
		}

		directx::Texture* PBRMaterial::GetSpecularMap()
		{
			directx::ShaderResourceDeclaration* declaration = FindResourceDeclaration("u_SpecularMap");

			if (!declaration)
			{
				// Dubug System
				std::cout << "PBRMaterial::GetSpecularMap()" << std::endl;
				exit(1);
			}

			unsigned int slot = declaration->GetRegister();

			return m_textures.size() > slot ? m_textures[slot] : nullptr;
		}

		directx::Texture* PBRMaterial::GetNormalMap()
		{
			directx::ShaderResourceDeclaration* declaration = FindResourceDeclaration("u_NormalMap");

			if (!declaration)
			{
				// Dubug System
				std::cout << "PBRMaterial::GetNormalMap()" << std::endl;
				exit(1);
			}

			unsigned int slot = declaration->GetRegister();

			return m_textures.size() > slot ? m_textures[slot] : nullptr;
		}

		directx::Texture* PBRMaterial::GetGlossMap()
		{
			directx::ShaderResourceDeclaration* declaration = FindResourceDeclaration("u_GlossMap");

			if (!declaration)
			{
				// Dubug System
				std::cout << "PBRMaterial::GetGlossMap()" << std::endl;
				exit(1);
			}

			unsigned int slot = declaration->GetRegister();

			return m_textures.size() > slot ? m_textures[slot] : nullptr;
		}
	}
}
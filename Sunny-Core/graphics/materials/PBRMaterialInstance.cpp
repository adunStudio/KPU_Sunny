#include "PBRMaterialInstance.h"

namespace sunny
{
	namespace graphics
	{
		PBRMaterialInstance::PBRMaterialInstance(PBRMaterial* material) : MaterialInstance(material)
		{

		}

		//void PBRMaterialInstance::SetEnviromentMap(directx::TextureCube* texture);

		// PBR Statics
		void PBRMaterialInstance::SetAlbedo(const maths::vec4& color)
		{
			SetUniform("u_AlbedoColor", color);
			SetUniform("u_UsingAlbedoMap", 0.0f);
		}

		void PBRMaterialInstance::SetSpecular(const maths::vec3& color)
		{
			SetUniform("u_SpecularColor", color);
			SetUniform("u_UsingSpecularMap", 0.0f);
		}

		void PBRMaterialInstance::SetGloss(float value)
		{
			SetUniform("u_GlossColor", value);
			SetUniform("u_UsingGlossMap", 0.0f);
		}

		void PBRMaterialInstance::UsingNormalMap(bool value)
		{
			SetUniform("u_UsingNormalMap", value ? 1.0f : 0.0f);
		}

		// PBR Maps
		void PBRMaterialInstance::SetAlbedoMap(directx::Texture2D* texture)
		{
			SetTexture("u_AlbedoMap", texture);
			SetUniform("u_UsingAlbedoMap", 1.0f);
		}

		void PBRMaterialInstance::SetSpecularMap(directx::Texture2D* texture)
		{
			SetTexture("u_SpecularMap", texture);
			SetUniform("u_UsingSpecularMap", 1.0f);
		}

		void PBRMaterialInstance::SetNormalMap(directx::Texture2D* texture)
		{
			SetTexture("u_NormalMap", texture);
			SetUniform("u_UsingNormalMap", 1.0f);
		}

		void PBRMaterialInstance::SetGlossMap(directx::Texture2D* texture)
		{
			SetUniform("u_GlossMap", texture);
			SetUniform("u_UsingGlossMap", 1.0f);
		}
	}
}
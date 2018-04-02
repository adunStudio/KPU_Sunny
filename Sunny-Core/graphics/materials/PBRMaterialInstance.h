#pragma once

#include "../../sunny.h"
#include "PBRMaterial.h"
#include "MaterialInstance.h"

namespace sunny
{
	namespace graphics
	{
		class PBRMaterialInstance : public MaterialInstance
		{
		public:
			PBRMaterialInstance(PBRMaterial* material);

			//void SetEnviromentMap(directx::TextureCube* texture);

			// PBR Statics
			void SetAlbedo(const maths::vec4& color);          
			void SetSpecular(const maths::vec3& color);   
			void SetGloss(float value);
			void UsingNormalMap(bool value);

			// PBR Maps
			void SetAlbedoMap(directx::Texture2D* texture);
			void SetSpecularMap(directx::Texture2D* texture);
			void SetNormalMap(directx::Texture2D* texture);
			void SetGlossMap(directx::Texture2D* texture);
		};
	}
}